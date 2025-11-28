# oxo-android-ben74 Contact Information Leakage with Native AES Encryption

## Challenge Details

### Description

This Android app demonstrates a sophisticated yet fundamentally vulnerable implementation involving:

- **Native AES Encryption (JNI/NDK)**: The app implements a custom AES-like cipher in C++ using Android NDK, making the vulnerability harder to discover through static analysis of the APK. The encryption key and IV are hardcoded in native code.
  
- **Contact Information Leakage**: The app requests READ_CONTACTS permission and exfiltrates contact data (names and phone numbers) to a remote server through multiple layers of abstraction.
  
- **Complex Architecture**: The app uses Room database for local caching, WorkManager for background sync, and multiple utility classes to obfuscate the data flow, creating a realistic modern Android app structure.

### Technical Vulnerability Details

**Architecture Complexity:**
- **Native Encryption Layer**: AES-256 CBC cipher implemented in C++ (native-lib.cpp), accessible only through JNI
- **Database Layer**: Room database caches data in sync queue before upload
- **Background Sync**: WorkManager schedules periodic data synchronization
- **Obfuscation Utilities**: Multiple helper classes (StringObfuscator, DataTransformer) add noise to the analysis
- **Metadata Tracking**: Comprehensive logging with checksums, fingerprints, and version info

**Why This Implementation is Still Vulnerable:**
- Hardcoded AES key (256-bit) embedded directly in C++ native code
- Hardcoded Initialization Vector (IV) instead of generating random IV per encryption
- Key and IV can be extracted through binary analysis of .so library (strings, IDA, Ghidra)
- No proper key derivation function (KDF) or secure key storage
- No integrity protection (HMAC/signature) or authenticated encryption
- Fixed IV defeats CBC mode security
- Complex architecture creates false sense of security

**The Attack Vector:**
1. App requests and obtains READ_CONTACTS permission
2. Contacts loaded and fingerprinted using SHA-256
3. Data queued in local Room database
4. Native library called via JNI to encrypt data
5. AES-256 CBC with hardcoded key and IV applied in C++
6. Encrypted data sent to remote server with metadata (checksum, fingerprint, version)
7. Background WorkManager syncs remaining queued data periodically
8. Attacker can:
   - Extract hardcoded key and IV from native .so library
   - Intercept network traffic and decrypt using extracted key/IV
   - Access local database to read queued unencrypted payloads

### Vulnerability Type and Category
- **Type:** Insecure Cryptography / Hardcoded Cryptographic Key
- **Category:** CWE-321: Use of Hard-coded Cryptographic Key
- **Type:** Insecure Cryptography / Improper Initialization
- **Category:** CWE-329: Not Using a Random IV with CBC Mode
- **Type:** Privacy Violation / Sensitive Data Exposure
- **Category:** CWE-359: Exposure of Private Personal Information to an Unauthorized Actor
- **Type:** Insecure Data Storage
- **Category:** CWE-312: Cleartext Storage of Sensitive Information (in database)

### Difficulty
Hard

## Build instructions

This project uses Android Studio with Kotlin, Jetpack Compose, and NDK for native code.

### Prerequisites
- Android Studio Arctic Fox or newer
- Android NDK (install via SDK Manager)
- CMake 3.22.1+ (install via SDK Manager)
- API Level 24+ target device/emulator

### Build Steps
1. Open the project in Android Studio
2. Ensure NDK and CMake are installed (Tools > SDK Manager > SDK Tools)
3. Sync Gradle files (File > Sync Project with Gradle Files)
4. Build the native library (Build > Make Project) - this compiles the C++ code
5. Build and deploy the app to an emulator or Android device with contacts
6. Grant READ_CONTACTS permission when prompted
7. Load contacts and observe the "encrypted" upload with native processing

### Native Library
The app includes a native C++ library (`libsecurelib.so`) that implements the encryption logic. The build process will:
- Compile `native-lib.cpp` for multiple ABIs (armeabi-v7a, arm64-v8a, x86, x86_64)
- Link the library with the app
- Make it available via JNI calls from Kotlin code

## Security Implications

**Real-world Impact:**
- Personal contact information exposure
- Privacy violations (names, phone numbers, contact metadata)
- Local database exposure (unencrypted payloads in sync queue)
- Background data exfiltration without user awareness

**Proper Mitigation:**
- Never hardcode cryptographic keys or IVs in source code or native libraries
- Use Android Keystore for secure key generation and storage
- Generate random IV for each encryption operation
- Use authenticated encryption modes (AES-GCM) instead of AES-CBC
- Use Android's standard cryptographic APIs (javax.crypto.Cipher)
- Implement proper key derivation (PBKDF2, Argon2) if deriving from password
- Add integrity protection with authenticated encryption
- Encrypt local database with SQLCipher if caching sensitive data
- Implement certificate pinning for network security
