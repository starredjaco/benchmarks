#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "SecureLib"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

const char* getObfuscatedKey() {
    static const char key[] = {'M', 'y', 'S', 'e', 'c', 'r', 'e', 't', 'K', 'e', 'y', '1', '2', '3', '\0'};
    return key;
}

std::string processData(const std::string& input, bool encode) {
    const char* key = getObfuscatedKey();
    size_t keyLen = strlen(key);
    std::string result;
    result.reserve(input.length());
    
    for (size_t i = 0; i < input.length(); ++i) {
        char processed = input[i] ^ key[i % keyLen];
        result.push_back(processed);
    }
    
    return result;
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_co_ostorlab_ben73_DataProtector_nativeTransform(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray input) {
    
    jsize inputLen = env->GetArrayLength(input);
    jbyte* inputBytes = env->GetByteArrayElements(input, nullptr);
    
    std::string inputStr(reinterpret_cast<char*>(inputBytes), inputLen);
    env->ReleaseByteArrayElements(input, inputBytes, JNI_ABORT);
    
    std::string transformed = processData(inputStr, true);
    
    jbyteArray result = env->NewByteArray(transformed.length());
    env->SetByteArrayRegion(result, 0, transformed.length(),
                           reinterpret_cast<const jbyte*>(transformed.c_str()));
    
    return result;
}

extern "C" JNIEXPORT jstring JNICALL
Java_co_ostorlab_ben73_DataProtector_nativeGetKeyHash(
        JNIEnv* env,
        jobject /* this */) {
    const char* key = getObfuscatedKey();
    
    unsigned int hash = 0;
    for (size_t i = 0; i < strlen(key); ++i) {
        hash = hash * 31 + key[i];
    }
    
    char hashStr[32];
    snprintf(hashStr, sizeof(hashStr), "%08x", hash);
    
    return env->NewStringUTF(hashStr);
}

extern "C" JNIEXPORT jstring JNICALL
Java_co_ostorlab_ben73_DataProtector_nativeGetVersion(
        JNIEnv* env,
        jobject /* this */) {
    return env->NewStringUTF("2.1.0-secure");
}
