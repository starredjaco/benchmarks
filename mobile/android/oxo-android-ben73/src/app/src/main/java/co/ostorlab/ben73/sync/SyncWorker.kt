package co.ostorlab.ben73.sync

import android.content.Context
import android.util.Log
import androidx.work.CoroutineWorker
import androidx.work.WorkerParameters
import co.ostorlab.ben73.ApiClient
import co.ostorlab.ben73.DataProtector
import co.ostorlab.ben73.data.AppDatabase
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class SyncWorker(
    context: Context,
    params: WorkerParameters
) : CoroutineWorker(context, params) {
    
    private val TAG = "SyncWorker"
    private val database = AppDatabase.getDatabase(context)
    
    override suspend fun doWork(): Result = withContext(Dispatchers.IO) {
        try {
            Log.d(TAG, "Starting sync operation")
            
            val pendingItems = database.syncDao().getPendingItems(limit = 5)
            
            if (pendingItems.isEmpty()) {
                Log.d(TAG, "No pending items to sync")
                return@withContext Result.success()
            }
            
            Log.d(TAG, "Found ${pendingItems.size} items to sync")
            
            var successCount = 0
            pendingItems.forEach { item ->
                try {
                    val protectedPayload = DataProtector.protect(item.payload)
                    Log.d(TAG, "Processing item ${item.id}, protected payload: ${protectedPayload.take(50)}...")
                    
                    val updatedItem = item.copy(
                        synced = true,
                        retryCount = item.retryCount + 1
                    )
                    database.syncDao().updateItem(updatedItem)
                    successCount++
                } catch (e: Exception) {
                    Log.e(TAG, "Failed to sync item ${item.id}", e)
                    val updatedItem = item.copy(retryCount = item.retryCount + 1)
                    database.syncDao().updateItem(updatedItem)
                }
            }
            
            Log.d(TAG, "Sync completed: $successCount/${pendingItems.size} succeeded")
            
            val weekAgo = System.currentTimeMillis() - (7 * 24 * 60 * 60 * 1000L)
            database.syncDao().cleanupSynced(weekAgo)
            
            Result.success()
        } catch (e: Exception) {
            Log.e(TAG, "Sync operation failed", e)
            Result.retry()
        }
    }
}
