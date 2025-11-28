# oxo-android-ben73 Contact Information Leakage with Custom Encryption

## Challenge Details

### Description

This Android app sample demonstrates a critical security vulnerability involving custom encryption and sensitive data leakage:

- **Custom/Weak Encryption**: The app uses a custom XOR-based encryption algorithm instead of standard cryptographic libraries (AES, RSA, etc.). This custom implementation is fundamentally flawed and easily reversible.
  
- **Contact Information Leakage**: The app requests READ_CONTACTS permission and exfiltrates contact data (names and phone numbers) to a remote server.
  
- **False Sense of Security**: While the data is "encrypted" before transmission, the weak custom encryption provides no real protection. An attacker intercepting the traffic can easily decrypt the data.

### Technical Vulnerability Details

**Why Custom Encryption is Dangerous:**
- Uses simple XOR cipher with hardcoded key
- No proper key derivation function (KDF)
- No integrity protection (HMAC/signature)
- No salt or initialization vector (IV)
- Algorithm is trivially reversible
- Gives false confidence that data is protected

**The Attack Vector:**
1. App requests and obtains READ_CONTACTS permission
2. User believes their contacts are "securely encrypted"
3. App loads contacts from device
4. Contacts are "encrypted" using weak XOR cipher with hardcoded key
5. "Encrypted" data is sent to remote server via HTTPS
6. Attacker with network access or server access can easily decrypt the data
7. Even though TLS protects the transport, the custom encryption is broken

### Vulnerability Type and Category
- **Type:** Insecure Cryptography / Use of Custom Cryptographic Algorithm
- **Category:** CWE-327: Use of a Broken or Risky Cryptographic Algorithm
- **Type:** Privacy Violation / Sensitive Data Exposure
- **Category:** CWE-359: Exposure of Private Personal Information to an Unauthorized Actor

### Difficulty
Medium

## Build instructions

This project uses Android Studio with Kotlin and Jetpack Compose.

1. Open the project in Android Studio
2. Update your SDK versions as required (compileSdkVersion >= 35 recommended)
3. Sync Gradle files
4. Build and deploy the app to an emulator or Android device with contacts
5. Grant READ_CONTACTS permission when prompted
6. Load contacts and observe the "encrypted" upload

## Security Implications

**Real-world Impact:**
- Personal contact information exposure
- Privacy violations (names, phone numbers)
- False sense of security from "encryption"
- Demonstrates why custom crypto should never be used
- Shows importance of using standard libraries (Android Keystore, javax.crypto)

**Proper Mitigation:**
- Never implement custom encryption algorithms
- Use Android's standard cryptographic APIs (javax.crypto.Cipher with AES-GCM)
- Use Android Keystore for key management
- Implement proper key derivation (PBKDF2, Argon2)
- Add integrity protection (HMAC-SHA256)
- Never hardcode encryption keys
- Consider if data transmission is even necessary
- Implement proper user consent and data minimization
