package co.ostorlab.ben73.data

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update

@Dao
interface SyncDao {
    @Query("SELECT * FROM sync_queue WHERE synced = 0 ORDER BY timestamp ASC LIMIT :limit")
    suspend fun getPendingItems(limit: Int = 10): List<SyncQueueItem>
    
    @Query("SELECT * FROM sync_queue WHERE id = :id")
    suspend fun getItemById(id: Long): SyncQueueItem?
    
    @Insert
    suspend fun insertItem(item: SyncQueueItem): Long
    
    @Update
    suspend fun updateItem(item: SyncQueueItem)
    
    @Query("DELETE FROM sync_queue WHERE synced = 1 AND timestamp < :beforeTimestamp")
    suspend fun cleanupSynced(beforeTimestamp: Long)
    
    @Query("SELECT * FROM sync_metadata WHERE key = :key")
    suspend fun getMetadata(key: String): SyncMetadata?
    
    @Insert
    suspend fun insertMetadata(metadata: SyncMetadata)
    
    @Update
    suspend fun updateMetadata(metadata: SyncMetadata)
}
