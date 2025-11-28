#include <jni.h>
#include <string>
#include <android/log.h>
#include <cstring>

#define LOG_TAG "SecureLib"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

const unsigned char* getHardcodedKey() {
    static const unsigned char key[32] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
        0x76, 0x2e, 0x71, 0x60, 0xf3, 0x8b, 0x4d, 0xa5,
        0x6a, 0x78, 0x4d, 0x90, 0x45, 0x19, 0x0c, 0xfe
    };
    return key;
}

const unsigned char* getHardcodedIV() {
    static const unsigned char iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    return iv;
}

void aes_encrypt_block(const unsigned char* input, unsigned char* output, 
                       const unsigned char* key) {
    unsigned char state[16];
    memcpy(state, input, 16);
    
    for (int i = 0; i < 16; ++i) {
        state[i] ^= key[i % 32];
    }
    
    for (int round = 0; round < 3; ++round) {
        for (int i = 0; i < 16; ++i) {
            state[i] = (state[i] << 1) | (state[i] >> 7);
            state[i] ^= key[(i + round) % 32];
        }
        
        for (int i = 0; i < 15; ++i) {
            state[i] ^= state[i + 1];
        }
    }
    
    memcpy(output, state, 16);
}

void aes_decrypt_block(const unsigned char* input, unsigned char* output,
                       const unsigned char* key) {
    unsigned char state[16];
    memcpy(state, input, 16);
    
    for (int round = 2; round >= 0; --round) {
        for (int i = 14; i >= 0; --i) {
            state[i] ^= state[i + 1];
        }
        
        for (int i = 15; i >= 0; --i) {
            state[i] ^= key[(i + round) % 32];
            state[i] = (state[i] >> 1) | (state[i] << 7);
        }
    }
    
    for (int i = 0; i < 16; ++i) {
        state[i] ^= key[i % 32];
    }
    
    memcpy(output, state, 16);
}

std::string aes_cbc_encrypt(const std::string& plaintext) {
    const unsigned char* key = getHardcodedKey();
    const unsigned char* iv = getHardcodedIV();
    
    size_t paddedLen = ((plaintext.length() + 15) / 16) * 16;
    std::string padded = plaintext;
    size_t paddingLen = paddedLen - plaintext.length();
    padded.append(paddingLen, static_cast<char>(paddingLen));
    
    std::string ciphertext;
    ciphertext.reserve(paddedLen + 16);
    ciphertext.append(reinterpret_cast<const char*>(iv), 16);
    
    unsigned char prev[16];
    memcpy(prev, iv, 16);
    
    for (size_t i = 0; i < paddedLen; i += 16) {
        unsigned char block[16];
        for (int j = 0; j < 16; ++j) {
            block[j] = padded[i + j] ^ prev[j];
        }
        
        unsigned char encrypted[16];
        aes_encrypt_block(block, encrypted, key);
        
        ciphertext.append(reinterpret_cast<const char*>(encrypted), 16);
        memcpy(prev, encrypted, 16);
    }
    
    return ciphertext;
}

std::string aes_cbc_decrypt(const std::string& ciphertext) {
    if (ciphertext.length() < 32 || ciphertext.length() % 16 != 0) {
        return "";
    }
    
    const unsigned char* key = getHardcodedKey();
    
    unsigned char iv[16];
    memcpy(iv, ciphertext.data(), 16);
    
    std::string plaintext;
    size_t dataLen = ciphertext.length() - 16;
    plaintext.reserve(dataLen);
    
    unsigned char prev[16];
    memcpy(prev, iv, 16);
    
    for (size_t i = 16; i < ciphertext.length(); i += 16) {
        unsigned char block[16];
        memcpy(block, ciphertext.data() + i, 16);
        
        unsigned char decrypted[16];
        aes_decrypt_block(block, decrypted, key);
        
        for (int j = 0; j < 16; ++j) {
            decrypted[j] ^= prev[j];
        }
        
        plaintext.append(reinterpret_cast<const char*>(decrypted), 16);
        memcpy(prev, block, 16);
    }
    
    if (!plaintext.empty()) {
        unsigned char paddingLen = plaintext[plaintext.length() - 1];
        if (paddingLen > 0 && paddingLen <= 16) {
            plaintext.resize(plaintext.length() - paddingLen);
        }
    }
    
    return plaintext;
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_co_ostorlab_ben74_DataProtector_nativeTransform(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray input,
        jboolean encrypt) {
    
    jsize inputLen = env->GetArrayLength(input);
    jbyte* inputBytes = env->GetByteArrayElements(input, nullptr);
    
    std::string inputStr(reinterpret_cast<char*>(inputBytes), inputLen);
    env->ReleaseByteArrayElements(input, inputBytes, JNI_ABORT);
    
    std::string result;
    if (encrypt) {
        result = aes_cbc_encrypt(inputStr);
    } else {
        result = aes_cbc_decrypt(inputStr);
    }
    
    jbyteArray output = env->NewByteArray(result.length());
    env->SetByteArrayRegion(output, 0, result.length(),
                           reinterpret_cast<const jbyte*>(result.c_str()));
    
    return output;
}

extern "C" JNIEXPORT jstring JNICALL
Java_co_ostorlab_ben74_DataProtector_nativeGetKeyHash(
        JNIEnv* env,
        jobject /* this */) {
    const unsigned char* key = getHardcodedKey();
    
    unsigned int hash = 0;
    for (int i = 0; i < 32; ++i) {
        hash = hash * 31 + key[i];
    }
    
    char hashStr[32];
    snprintf(hashStr, sizeof(hashStr), "%08x", hash);
    
    return env->NewStringUTF(hashStr);
}

extern "C" JNIEXPORT jstring JNICALL
Java_co_ostorlab_ben74_DataProtector_nativeGetVersion(
        JNIEnv* env,
        jobject /* this */) {
    return env->NewStringUTF("2.2.0-aes256");
}
