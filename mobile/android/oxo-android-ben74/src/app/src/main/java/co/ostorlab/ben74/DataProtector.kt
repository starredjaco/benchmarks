package co.ostorlab.ben74

import android.util.Base64
import android.util.Log

object DataProtector {
    
    private const val TAG = "DataProtector"
    
    init {
        try {
            System.loadLibrary("securelib")
            Log.d(TAG, "Native library loaded successfully")
            Log.d(TAG, "Version: ${nativeGetVersion()}")
            Log.d(TAG, "Key hash: ${nativeGetKeyHash()}")
        } catch (e: UnsatisfiedLinkError) {
            Log.e(TAG, "Failed to load native library", e)
        }
    }
    
    private external fun nativeTransform(input: ByteArray, encrypt: Boolean): ByteArray
    private external fun nativeGetKeyHash(): String
    private external fun nativeGetVersion(): String
    
    fun protect(data: String): String {
        val dataBytes = data.toByteArray(Charsets.UTF_8)
        val transformed = nativeTransform(dataBytes, true)
        return Base64.encodeToString(transformed, Base64.NO_WRAP)
    }
    
    fun unprotect(protectedData: String): String {
        val decoded = Base64.decode(protectedData, Base64.NO_WRAP)
        val original = nativeTransform(decoded, false)
        return String(original, Charsets.UTF_8)
    }
    
    fun getSalt(): String {
        return "Key stored in native code"
    }
    
    fun getKeyHash(): String = nativeGetKeyHash()
    
    fun getVersion(): String = nativeGetVersion()
}
