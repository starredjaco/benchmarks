package co.ostorlab.ben73

import android.util.Base64

object DataProtector {
    
    private const val SALT = "MySecretKey123"
    
    fun protect(data: String): String {
        val keyBytes = SALT.toByteArray()
        val dataBytes = data.toByteArray()
        val processed = ByteArray(dataBytes.size)
        
        for (i in dataBytes.indices) {
            processed[i] = (dataBytes[i].toInt() xor keyBytes[i % keyBytes.size].toInt()).toByte()
        }
        
        return Base64.encodeToString(processed, Base64.NO_WRAP)
    }
    
    fun unprotect(protectedData: String): String {
        val processed = Base64.decode(protectedData, Base64.NO_WRAP)
        val keyBytes = SALT.toByteArray()
        val original = ByteArray(processed.size)
        
        for (i in processed.indices) {
            original[i] = (processed[i].toInt() xor keyBytes[i % keyBytes.size].toInt()).toByte()
        }
        
        return String(original)
    }
    
    fun getSalt(): String = SALT
}
