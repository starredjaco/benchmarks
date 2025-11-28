package co.ostorlab.ben74.util

import android.util.Log
import java.security.MessageDigest
import java.util.UUID

object StringObfuscator {
    
    private const val TAG = "StringObfuscator"
    
    fun generateFingerprint(data: String): String {
        return try {
            val digest = MessageDigest.getInstance("SHA-256")
            val hash = digest.digest(data.toByteArray())
            hash.joinToString("") { "%02x".format(it) }.take(16)
        } catch (e: Exception) {
            Log.e(TAG, "Failed to generate fingerprint", e)
            UUID.randomUUID().toString().replace("-", "").take(16)
        }
    }
    
    fun encodeMetadata(key: String, value: String): String {
        val combined = "$key:$value:${System.currentTimeMillis()}"
        return combined.toByteArray().joinToString("") { "%02x".format(it) }
    }
    
    fun createChecksum(data: ByteArray): String {
        var checksum = 0L
        data.forEach { byte ->
            checksum = (checksum * 31 + byte.toInt()) and 0xFFFFFFFF
        }
        return checksum.toString(16).padStart(8, '0')
    }
}

object DataTransformer {
    
    fun chunk(data: String, size: Int = 100): List<String> {
        return data.chunked(size)
    }
    
    fun compress(data: String): String {
        return data.replace("\\s+".toRegex(), " ").trim()
    }
    
    fun addNoise(data: String, level: Int = 3): String {
        val noise = (1..level).joinToString("") { 
            ('a'..'z').random().toString() 
        }
        return "$data$noise"
    }
}
