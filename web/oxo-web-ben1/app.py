import flask
import flask_migrate
import config
import jira_client
import models
import db_helpers
import logging
import io

# Set up logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Initialize Flask app
app = flask.Flask(__name__)
app.config.from_object(config.Config)

# Initialize database
models.db.init_app(app)
migrate = flask_migrate.Migrate(app, models.db)

# Initialize Jira client
try:
    config.Config.validate()
    jira = jira_client.JiraClient(
        jira_url=config.Config.JIRA_URL,
        email=config.Config.JIRA_EMAIL,
        api_token=config.Config.JIRA_API_TOKEN
    )
    logger.info("Jira client initialized successfully")
except ValueError as e:
    logger.error(f"Configuration error: {e}")
    jira = None


@app.route('/')
def index():
    """Home page"""
    connection_status = None

    if jira is not None:
        connection_status = jira.test_connection()

    return flask.render_template('index.html', connection_status=connection_status)


@app.route('/projects')
def projects():
    """Display all projects"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    projects_list = jira.get_projects()

    if isinstance(projects_list, dict) and 'error' in projects_list:
        flask.flash(f'Error fetching projects: {projects_list["error"]}', 'error')
        projects_list = []
    else:
        # Cache projects in database
        for project_data in projects_list:
            db_helpers.cache_project(project_data)
        logger.info(f"Cached {len(projects_list)} projects")

    return flask.render_template('projects.html', projects=projects_list)


@app.route('/project/<project_key>')
def project_detail(project_key):
    """Display project details"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    project = jira.get_project(project_key)

    if isinstance(project, dict) and 'error' in project:
        flask.flash(f'Error fetching project: {project["error"]}', 'error')
        return flask.redirect(flask.url_for('projects'))
    else:
        # Cache project in database
        db_helpers.cache_project(project)
        logger.info(f"Cached project: {project_key}")

    return flask.render_template('project_detail.html', project=project)


@app.route('/issues')
@app.route('/issues/<project_key>')
def issues(project_key=None):
    """Display issues, optionally filtered by project"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    # Get query parameters
    max_results = flask.request.args.get('max_results', 50, type=int)

    # Fetch issues
    issues_data = jira.get_issues(project_key=project_key, max_results=max_results)

    # Cache issues in database
    if 'issues' in issues_data and len(issues_data['issues']) > 0:
        cached_count = db_helpers.cache_multiple_issues(issues_data['issues'])
        logger.info(f"Cached {cached_count} issues")

    # Get project info if filtering by project
    project = None
    if project_key is not None and project_key != '':
        project = jira.get_project(project_key)
        if project is not None and (not isinstance(project, dict) or 'error' not in project):
            db_helpers.cache_project(project)

    return flask.render_template(
        'issues.html',
        issues=issues_data.get('issues', []),
        total=issues_data.get('total', 0),
        project=project,
        project_key=project_key
    )


@app.route('/issue/<issue_key>')
def issue_detail(issue_key):
    """Display detailed information about a specific issue"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    issue = jira.get_issue(issue_key)

    if isinstance(issue, dict) and 'error' in issue:
        flask.flash(f'Error fetching issue: {issue["error"]}', 'error')
        return flask.redirect(flask.url_for('issues'))
    else:
        # Cache issue in database
        db_helpers.cache_issue(issue)
        logger.info(f"Cached issue: {issue_key}")

    # Get comments
    comments = jira.get_issue_comments(issue_key)

    return flask.render_template('issue_detail.html', issue=issue, comments=comments)


@app.route('/search')
def search():
    """Search issues using JQL"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    jql = flask.request.args.get('jql', '')
    max_results = flask.request.args.get('max_results', 50, type=int)

    issues_data = None

    if jql is not None and jql != '':
        issues_data = jira.search_issues(jql=jql, max_results=max_results)

        # Cache search results
        if issues_data is not None and 'issues' in issues_data and len(issues_data['issues']) > 0:
            cached_count = db_helpers.cache_multiple_issues(issues_data['issues'])
            logger.info(f"Cached {cached_count} issues from search")

    return flask.render_template(
        'search.html',
        jql=jql,
        issues=issues_data.get('issues', []) if issues_data is not None else [],
        total=issues_data.get('total', 0) if issues_data is not None else 0
    )


@app.route('/attachment/<attachment_id>')
def download_attachment(attachment_id):
    """Download attachment from Jira"""
    if jira is None:
        flask.flash('Backend service not configured. Please check your .env file.', 'error')
        return flask.redirect(flask.url_for('index'))

    # Get attachment metadata
    attachment = jira.get_attachment(attachment_id)

    if isinstance(attachment, dict) and 'error' in attachment:
        flask.flash(f'Error fetching attachment: {attachment["error"]}', 'error')
        return flask.redirect(flask.url_for('index'))

    # Get the content URL and filename
    content_url = attachment.get('content')
    filename = attachment.get('filename', 'download')

    if content_url is None or content_url == '':
        flask.flash('Attachment content URL not found', 'error')
        return flask.redirect(flask.url_for('index'))

    # Download the attachment content
    content, content_type = jira.download_attachment_content(content_url)

    if content is None:
        flask.flash('Failed to download attachment', 'error')
        return flask.redirect(flask.url_for('index'))

    # Return the file
    return flask.send_file(
        io.BytesIO(content),
        mimetype=content_type,
        as_attachment=True,
        download_name=filename
    )


@app.route('/api/test-connection')
def api_test_connection():
    """API endpoint to test connection"""
    if jira is None:
        return flask.jsonify({
            'success': False,
            'message': 'Backend service not configured'
        }), 500

    result = jira.test_connection()
    status_code = 200 if result.get('success') else 500

    return flask.jsonify(result), status_code


@app.route('/api/projects')
def api_projects():
    """API endpoint to get all projects"""
    if jira is None:
        return flask.jsonify({'error': 'Backend service not configured'}), 500

    projects_list = jira.get_projects()
    return flask.jsonify(projects_list)


@app.route('/api/issues')
def api_issues():
    """API endpoint to get issues"""
    if jira is None:
        return flask.jsonify({'error': 'Backend service not configured'}), 500

    project_key = flask.request.args.get('project')
    max_results = flask.request.args.get('max_results', 50, type=int)
    jql = flask.request.args.get('jql')

    issues_data = jira.get_issues(
        project_key=project_key,
        max_results=max_results,
        jql=jql
    )

    return flask.jsonify(issues_data)


@app.route('/api/issue/<issue_key>')
def api_issue(issue_key):
    """API endpoint to get a specific issue"""
    if jira is None:
        return flask.jsonify({'error': 'Backend service not configured'}), 500

    issue = jira.get_issue(issue_key)
    return flask.jsonify(issue)


# Error handlers
@app.errorhandler(404)
def not_found(error):
    """Handle 404 errors"""
    return flask.render_template('404.html'), 404


@app.errorhandler(500)
def internal_error(error):
    """Handle 500 errors"""
    logger.error(f"Internal error: {error}")
    return flask.render_template('500.html'), 500


# Template filters
@app.template_filter('format_date')
def format_date(date_string):
    """Format ISO date string to readable format"""
    if date_string is None or date_string == '':
        return 'N/A'

    try:
        from datetime import datetime
        dt = datetime.fromisoformat(date_string.replace('Z', '+00:00'))
        return dt.strftime('%Y-%m-%d %H:%M')
    except Exception:
        return date_string


if __name__ == '__main__':
    if jira is None:
        logger.warning(
            "Backend service not configured. Copy .env.example to .env, set Jira credentials, then restart."
        )
    else:
        logger.info(
            "Project Hub running at http://127.0.0.1:5000 (Jira host: %s)",
            config.Config.JIRA_URL,
        )

    app.run(debug=config.Config.DEBUG, host='0.0.0.0', port=5000)
