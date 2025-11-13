from flask import Flask, render_template, redirect, url_for, session, request, make_response, jsonify, flash
from onelogin.saml2.auth import OneLogin_Saml2_Auth
from onelogin.saml2.utils import OneLogin_Saml2_Utils
from functools import wraps
import os
import json
import tempfile
import logging
from werkzeug.middleware.proxy_fix import ProxyFix
from models import db, SAMLConfig, Todo

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = Flask(__name__)
app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY', 'dev-secret-key-change-in-production')

data_dir = os.path.join(os.path.dirname(__file__), 'data')
os.makedirs(data_dir, exist_ok=True)

app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{os.path.join(data_dir, "saml_config.db")}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db.init_app(app)

app.wsgi_app = ProxyFix(app.wsgi_app, x_proto=1, x_host=1)

with app.app_context():
    db.create_all()


def is_saml_enabled():
    """Check if SAML is enabled"""
    config = SAMLConfig.get_or_create()
    return config.enabled


def init_saml_auth(req):
    """Initialize SAML authentication using database config"""
    config = SAMLConfig.get_or_create()
    settings_dict = config.to_settings_dict()

    temp_dir = tempfile.mkdtemp()
    settings_file = os.path.join(temp_dir, 'settings.json')

    try:
        with open(settings_file, 'w') as f:
            json.dump(settings_dict, f)

        auth = OneLogin_Saml2_Auth(req, custom_base_path=temp_dir)
        return auth
    finally:
        if os.path.exists(settings_file):
            os.unlink(settings_file)
        if os.path.exists(temp_dir):
            os.rmdir(temp_dir)


def prepare_flask_request(request):
    """Prepare Flask request for SAML library"""
    forwarded_proto = request.headers.get('X-Forwarded-Proto', request.scheme)

    http_host = request.headers.get('X-Forwarded-Host', request.host)

    if ':' in http_host:
        host_without_port = http_host.split(':')[0]
        http_host = host_without_port

    url_data = {
        'https': 'on' if forwarded_proto == 'https' else 'off',
        'http_host': http_host,
        'server_port': 443 if forwarded_proto == 'https' else 80,
        'script_name': request.path,
        'get_data': request.args.copy(),
        'post_data': request.form.copy()
    }
    return url_data


def login_required(f):
    """Decorator to protect routes that require authentication"""
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'logged_in' not in session:
            return redirect(url_for('login'))

        if is_saml_enabled() and 'saml_authenticated' not in session:
            return redirect(url_for('login'))

        return f(*args, **kwargs)
    return decorated_function


@app.route('/')
def index():
    """Home page"""
    if 'logged_in' in session:
        return redirect(url_for('dashboard'))
    return render_template('index.html', saml_enabled=is_saml_enabled())


@app.route('/login', methods=['GET', 'POST'])
def login():
    """Login page - simple or SAML"""
    if 'logged_in' in session:
        return redirect(url_for('dashboard'))

    if request.method == 'POST':
        if not is_saml_enabled():
            session['logged_in'] = True
            session['username'] = request.form.get('username', 'Guest')
            return redirect(url_for('dashboard'))

    return render_template('login.html', saml_enabled=is_saml_enabled())


@app.route('/simple-login', methods=['POST'])
def simple_login():
    """Simple login without password"""
    if not is_saml_enabled():
        session['logged_in'] = True
        session['username'] = request.form.get('username', 'Guest')
        return redirect(url_for('dashboard'))
    else:
        return redirect(url_for('login'))


@app.route('/saml/login')
def saml_login():
    """SAML login endpoint - redirects to IdP"""
    if not is_saml_enabled():
        return redirect(url_for('login'))

    try:
        req = prepare_flask_request(request)
        auth = init_saml_auth(req)
        return redirect(auth.login(return_to=url_for('dashboard', _external=True)))
    except Exception as e:
        return render_template('error.html', error=f'SAML initialization failed: {str(e)}'), 500


@app.route('/saml/acs', methods=['POST'])
def saml_acs():
    """SAML Assertion Consumer Service - handles IdP response"""
    try:

        req = prepare_flask_request(request)
        auth = init_saml_auth(req)
        auth.process_response()

        errors = auth.get_errors()

        if len(errors) == 0:
            if auth.is_authenticated():
                session['logged_in'] = True
                session['saml_authenticated'] = True
                session['saml_attributes'] = auth.get_attributes()
                session['saml_nameid'] = auth.get_nameid()
                session['saml_nameid_format'] = auth.get_nameid_format()
                session['saml_session_index'] = auth.get_session_index()

                if 'RelayState' in request.form:
                    self_url = OneLogin_Saml2_Utils.get_self_url(req)
                    if self_url != request.form['RelayState']:
                        return redirect(auth.redirect_to(request.form['RelayState']))

                return redirect(url_for('dashboard'))
            else:
                logger.warning("SAML authentication failed: not authenticated")
                return render_template('error.html', error='Not authenticated'), 401
        else:
            error_msg = ', '.join(errors)
            error_reason = auth.get_last_error_reason()
            logger.error(f"SAML Error: {error_msg}, Reason: {error_reason}")
            return render_template('error.html', error=f'{error_msg}: {error_reason}'), 401

    except Exception as e:
        logger.exception(f"Exception in SAML ACS: {str(e)}")
        return render_template('error.html', error=str(e)), 500


@app.route('/saml/metadata')
def saml_metadata():
    """SAML metadata endpoint"""
    req = prepare_flask_request(request)
    auth = init_saml_auth(req)
    settings = auth.get_settings()
    metadata = settings.get_sp_metadata()
    errors = settings.validate_metadata(metadata)

    if len(errors) == 0:
        resp = make_response(metadata, 200)
        resp.headers['Content-Type'] = 'text/xml'
        return resp
    else:
        return render_template('error.html', error=', '.join(errors)), 500


def get_current_username():
    """Get the current logged-in username"""
    if 'saml_authenticated' in session:
        user_attrs = session.get('saml_attributes', {})
        email = user_attrs.get('http://schemas.xmlsoap.org/ws/2005/05/identity/claims/emailaddress', [''])[0]
        name = user_attrs.get('http://schemas.xmlsoap.org/ws/2005/05/identity/claims/name', [''])[0]
        nickname = user_attrs.get('http://schemas.auth0.com/nickname', [''])[0]
        return nickname or email or name or session.get('saml_nameid', 'User')
    return session.get('username', 'User')


@app.route('/dashboard')
@login_required
def dashboard():
    """Protected dashboard page"""
    user_attrs = session.get('saml_attributes', {})
    username = get_current_username()
    saml_config = SAMLConfig.get_or_create()

    todos = Todo.query.filter_by(username=username).order_by(Todo.created_at.desc()).all()

    return render_template('dashboard.html',
                         user=user_attrs,
                         username=username,
                         saml_enabled=saml_config.enabled,
                         saml_configured=saml_config.configured,
                         todos=todos)


@app.route('/logout')
@login_required
def logout():
    """Logout endpoint - local logout only"""
    session.clear()
    return redirect(url_for('index'))


@app.route('/saml/logout')
@login_required
def saml_logout():
    """SAML logout endpoint - initiates SLO"""
    try:
        req = prepare_flask_request(request)
        auth = init_saml_auth(req)

        name_id = session.get('saml_nameid')
        session_index = session.get('saml_session_index')
        name_id_format = session.get('saml_nameid_format')

        session.clear()

        return redirect(auth.logout(name_id=name_id, session_index=session_index, nq=name_id_format))
    except Exception as e:
        logger.warning(f"SAML logout error: {str(e)}")
        session.clear()
        return redirect(url_for('index'))


@app.route('/saml/sls', methods=['GET', 'POST'])
def saml_sls():
    """SAML Single Logout Service"""
    req = prepare_flask_request(request)
    auth = init_saml_auth(req)

    url = auth.process_slo()
    errors = auth.get_errors()

    session.clear()

    if len(errors) == 0:
        if url is not None:
            return redirect(url)
        else:
            return redirect(url_for('index'))
    else:
        return render_template('error.html', error=', '.join(errors)), 500


@app.route('/saml-config')
@login_required
def saml_config_page():
    """SAML configuration page"""
    saml_config = SAMLConfig.get_or_create()
    saml_settings = saml_config.to_settings_dict()

    return render_template('saml_config.html',
                         saml_config={'enabled': saml_config.enabled, 'configured': saml_config.configured},
                         saml_settings=saml_settings)


@app.route('/saml-config/save', methods=['POST'])
@login_required
def save_saml_config():
    """Save SAML configuration"""
    try:
        idp_entity_id = request.form.get('idp_entity_id', '')
        idp_sso_url = request.form.get('idp_sso_url', '')
        idp_slo_url = request.form.get('idp_slo_url', '')
        idp_cert = request.form.get('idp_cert', '')

        config = SAMLConfig.get_or_create()

        config.idp_entity_id = idp_entity_id
        config.idp_sso_url = idp_sso_url
        config.idp_slo_url = idp_slo_url
        config.idp_x509_cert = idp_cert
        config.configured = True

        db.session.commit()

        return jsonify({'success': True, 'message': 'SAML configuration saved successfully'})

    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/saml-config/toggle', methods=['POST'])
@login_required
def toggle_saml():
    """Enable or disable SAML authentication"""
    try:
        data = request.get_json()
        enable = data.get('enable', False)

        config = SAMLConfig.get_or_create()

        if enable and not config.configured:
            return jsonify({'success': False, 'message': 'Please configure SAML settings first'}), 400

        config.enabled = enable
        db.session.commit()

        if enable:
            session.clear()
            return jsonify({'success': True, 'message': 'SAML enabled. Please log in again.', 'logout': True})
        else:
            return jsonify({'success': True, 'message': 'SAML disabled. Simple login is now active.'})

    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/todos', methods=['GET'])
@login_required
def get_todos():
    """Get all todos for current user"""
    username = get_current_username()
    todos = Todo.query.filter_by(username=username).order_by(Todo.created_at.desc()).all()
    return jsonify({'success': True, 'todos': [todo.to_dict() for todo in todos]})


@app.route('/todos', methods=['POST'])
@login_required
def create_todo():
    """Create a new todo"""
    try:
        data = request.get_json()
        title = data.get('title', '').strip()

        if not title:
            return jsonify({'success': False, 'message': 'Title is required'}), 400

        username = get_current_username()
        todo = Todo(username=username, title=title)
        db.session.add(todo)
        db.session.commit()

        return jsonify({'success': True, 'todo': todo.to_dict()})

    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/todos/<int:todo_id>', methods=['PUT'])
@login_required
def update_todo(todo_id):
    """Update a todo (toggle completion or edit title)"""
    try:
        username = get_current_username()
        todo = Todo.query.filter_by(id=todo_id, username=username).first()

        if not todo:
            return jsonify({'success': False, 'message': 'Todo not found'}), 404

        data = request.get_json()

        if 'completed' in data:
            todo.completed = data['completed']

        if 'title' in data:
            title = data['title'].strip()
            if not title:
                return jsonify({'success': False, 'message': 'Title cannot be empty'}), 400
            todo.title = title

        db.session.commit()

        return jsonify({'success': True, 'todo': todo.to_dict()})

    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/todos/<int:todo_id>', methods=['DELETE'])
@login_required
def delete_todo(todo_id):
    """Delete a todo"""
    try:
        username = get_current_username()
        todo = Todo.query.filter_by(id=todo_id, username=username).first()

        if not todo:
            return jsonify({'success': False, 'message': 'Todo not found'}), 404

        db.session.delete(todo)
        db.session.commit()

        return jsonify({'success': True, 'message': 'Todo deleted'})

    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
