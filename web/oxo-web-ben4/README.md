# Complex Structured postMessage Vulnerability Benchmark

### Overview

This benchmark demonstrates a more sophisticated postMessage vulnerability where the victim application expects structured message objects with specific fields and values. this victim validates message structure (type, action, data fields) but still fails to validate event.origin and uses innerHTML for rendering, creating a DOM-based XSS vulnerability that requires crafting properly structured exploit payloads.

The victim (bank.html) implements a transaction status system that accepts messages with a specific schema. An attacker page embeds the victim in an iframe and sends a crafted structured message that passes the validation checks but contains malicious HTML/JavaScript in the data fields. Because the victim uses innerHTML without sanitization and does not verify event.origin, the attacker's code executes in the victim's origin and can exfiltrate cookies, localStorage, and sessionStorage.

### Critical Vulnerability
Structured postMessage with Missing Origin Validation → DOM XSS (CRITICAL)

Description
The victim page implements message validation that checks for:
- Message must be an object
- Message must have type field equal to 'TRANSACTION_RESPONSE'
- Message must have action field ('update_status' or 'display_notification')
- Message must have data object with specific fields (status or message)

However, the implementation has critical flaws:

```javascript
window.addEventListener('message', function(event){
  const msg = event.data;
  
  // VULNERABLE: No event.origin validation
  
  if (typeof msg !== 'object' || msg === null) return;
  if (msg.type !== 'TRANSACTION_RESPONSE') return;
  
  if (msg.action === 'update_status') {
    if (msg.data && msg.data.status) {
      // VULNERABLE: innerHTML with no sanitization
      statusOutput.innerHTML = msg.data.status;
    }
  } else if (msg.action === 'display_notification') {
    if (msg.data && msg.data.message) {
      // VULNERABLE: innerHTML with no sanitization
      statusOutput.innerHTML = msg.data.message;
    }
  }
});
```

An attacker can craft a message object that satisfies all structural requirements but contains malicious HTML/JavaScript in the data.status or data.message fields. When the victim renders this using innerHTML, the attacker's script executes in the victim's origin.

### Impact / Risk

Full DOM execution in victim origin with access to:
- Non-HttpOnly cookies (session tokens, authentication cookies)
- localStorage (API keys, user preferences, cached data)
- sessionStorage (temporary session data, CSRF tokens)
- Full DOM access for UI manipulation and credential harvesting

The structured nature of the vulnerability makes it harder to detect with simple pattern matching, as the code appears to implement validation. However, the validation only checks structure, not origin or content safety.

Automatic exploitation is possible via iframe.onload with no user interaction required.

Severity: Critical

### PoC (what to run)

Set up three components on separate local servers:

1. bank.html — victim (serve from http://localhost:5500/bank.html)
   - Contains transaction submission component
   - Contains vulnerable message listener with structural validation but no origin check
   - Uses innerHTML to render status messages

2. attacker.html — attacker (serve from http://localhost:5501/attacker.html)
   - Embeds victim in iframe
   - Sends properly structured malicious message on iframe.onload
   - Payload crafted to pass all validation checks

3. collector.js — exfiltration endpoint (run on http://localhost:9000)
   - Simple Express server to receive stolen data
   - Logs cookies, localStorage, and sessionStorage

Exploit payload structure:

```javascript
const maliciousMessage = {
  type: 'TRANSACTION_RESPONSE',        // Required type
  action: 'update_status',              // Valid action
  data: {
    status: '<img src=x onerror="...">' // XSS payload in expected field
  }
};
```

The payload satisfies all structural checks but contains executable code that exfiltrates sensitive data to the collector endpoint.

### Detection Challenges

This vulnerability is harder to detect than simple postMessage XSS because:

1. Code appears to implement validation (type checking, field validation)
2. Multiple conditional branches make static analysis more complex
3. The vulnerability requires understanding the full message flow
4. Automated scanners may not craft properly structured exploit payloads
5. The innerHTML sink is nested within multiple conditional blocks

Security reviewers must recognize that structural validation alone is insufficient without origin validation and output sanitization.

### Mitigations & Secure Patterns

Apply these defensive measures to fix the vulnerability:

1. Validate event.origin with strict allowlist:
```javascript
window.addEventListener('message', (event) => {
  const ALLOWED_ORIGINS = ['https://trusted-partner.example.com'];
  if (!ALLOWED_ORIGINS.includes(event.origin)) {
    console.warn('Rejected message from unauthorized origin:', event.origin);
    return;
  }
  // Continue processing...
});
```

2. Use textContent instead of innerHTML for untrusted data:
```javascript
if (msg.action === 'update_status') {
  if (msg.data && msg.data.status) {
    statusOutput.textContent = msg.data.status; // Safe
  }
}
```

3. If HTML rendering is required, sanitize with DOMPurify:
```javascript
if (msg.data && msg.data.status) {
  const clean = DOMPurify.sanitize(msg.data.status);
  statusOutput.innerHTML = clean;
}
```

4. Implement message signing/authentication for sensitive operations:
```javascript
function verifyMessageSignature(msg, signature) {
  // Implement HMAC or similar verification
  // Only process messages with valid signatures
}
```

5. Use structured cloning and validate all nested fields:
```javascript
function isValidTransactionResponse(msg) {
  return msg &&
         typeof msg === 'object' &&
         msg.type === 'TRANSACTION_RESPONSE' &&
         ['update_status', 'display_notification'].includes(msg.action) &&
         msg.data &&
         typeof msg.data === 'object' &&
         (typeof msg.data.status === 'string' || typeof msg.data.message === 'string');
}
```

6. Apply defense in depth:
   - Set HttpOnly and Secure flags on sensitive cookies
   - Implement Content Security Policy to restrict inline script execution
   - Use SameSite cookie attribute to prevent CSRF
   - Minimize data stored in localStorage/sessionStorage
   - Implement iframe sandbox attributes where appropriate

7. Security testing:
   - Test with malformed and malicious structured messages
   - Verify origin validation with various origin spoofing attempts
   - Audit all postMessage listeners for proper validation
   - Use CSP reporting to detect injection attempts

### References

- OWASP postMessage Security: https://cheatsheetseries.owasp.org/cheatsheets/HTML5_Security_Cheat_Sheet.html#postmessage
- MDN postMessage: https://developer.mozilla.org/en-US/docs/Web/API/Window/postMessage
- PortSwigger Web Message Manipulation: https://portswigger.net/web-security/dom-based/controlling-the-web-message-source
- DOMPurify: https://github.com/cure53/DOMPurify
