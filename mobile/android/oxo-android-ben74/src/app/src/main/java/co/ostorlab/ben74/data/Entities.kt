package co.ostorlab.ben74.data

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "sync_queue")
data class SyncQueueItem(
    @PrimaryKey(autoGenerate = true)
    val id: Long = 0,
    val payload: String,
    val timestamp: Long,
    val synced: Boolean = false,
    val retryCount: Int = 0
)

@Entity(tableName = "sync_metadata")
data class SyncMetadata(
    @PrimaryKey
    val key: String,
    val value: String,
    val updatedAt: Long
)
