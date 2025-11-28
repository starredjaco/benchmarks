package co.ostorlab.ben73

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.provider.ContactsContract
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat
import co.ostorlab.ben73.ui.theme.SyncManagerTheme
import kotlinx.coroutines.launch

class MainActivity : ComponentActivity() {
    
    private val TAG = "MainActivity"
    
    private val requestPermissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestPermission()
    ) { isGranted: Boolean ->
        if (isGranted) {
            Log.d(TAG, "Permission granted")
        } else {
            Log.d(TAG, "Permission denied")
        }
    }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            SyncManagerTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    MainScreen(
                        onRequestPermission = { requestDataPermission() },
                        onLoadData = { loadUserData() },
                        onSyncData = { data -> syncToCloud(data) },
                        hasPermission = { checkDataPermission() }
                    )
                }
            }
        }
    }
    
    private fun requestDataPermission() {
        requestPermissionLauncher.launch(Manifest.permission.READ_CONTACTS)
    }
    
    private fun checkDataPermission(): Boolean {
        return ContextCompat.checkSelfPermission(
            this,
            Manifest.permission.READ_CONTACTS
        ) == PackageManager.PERMISSION_GRANTED
    }
    
    private fun loadUserData(): List<UserData> {
        val dataList = mutableListOf<UserData>()
        
        if (!checkDataPermission()) {
            Log.w(TAG, "No permission to read data")
            return dataList
        }
        
        try {
            val cursor = contentResolver.query(
                ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                arrayOf(
                    ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME,
                    ContactsContract.CommonDataKinds.Phone.NUMBER
                ),
                null,
                null,
                ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME + " ASC"
            )
            
            cursor?.use {
                val nameIndex = it.getColumnIndex(ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME)
                val numberIndex = it.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER)
                
                while (it.moveToNext() && dataList.size < 20) {
                    val name = it.getString(nameIndex) ?: "Unknown"
                    val number = it.getString(numberIndex) ?: "No number"
                    dataList.add(UserData(name, number))
                }
            }
            
            Log.d(TAG, "Loaded ${dataList.size} items")
        } catch (e: Exception) {
            Log.e(TAG, "Error loading data", e)
        }
        
        return dataList
    }
    
    private suspend fun syncToCloud(data: List<UserData>): Boolean {
        return ApiClient.uploadData(data)
    }
}

@Composable
fun MainScreen(
    onRequestPermission: () -> Unit,
    onLoadData: () -> List<UserData>,
    onSyncData: suspend (List<UserData>) -> Boolean,
    hasPermission: () -> Boolean
) {
    var items by remember { mutableStateOf<List<UserData>>(emptyList()) }
    var statusMessage by remember { mutableStateOf("") }
    var isLoading by remember { mutableStateOf(false) }
    val scope = rememberCoroutineScope()
    
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = "Sync Manager",
            style = MaterialTheme.typography.headlineMedium,
            modifier = Modifier.padding(vertical = 16.dp)
        )
        
        Text(
            text = "Sync your data securely to the cloud",
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant,
            modifier = Modifier.padding(bottom = 24.dp)
        )
        
        if (!hasPermission()) {
            Button(
                onClick = onRequestPermission,
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 8.dp)
            ) {
                Text("Grant Permission")
            }
        } else {
            Button(
                onClick = {
                    isLoading = true
                    items = onLoadData()
                    statusMessage = "Loaded ${items.size} items"
                    isLoading = false
                },
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 8.dp),
                enabled = !isLoading
            ) {
                Text("Load Data")
            }
            
            if (items.isNotEmpty()) {
                Button(
                    onClick = {
                        scope.launch {
                            isLoading = true
                            statusMessage = "Securing and uploading data..."
                            val success = onSyncData(items)
                            statusMessage = if (success) {
                                "✓ Data uploaded successfully!"
                            } else {
                                "✗ Upload failed"
                            }
                            isLoading = false
                        }
                    },
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(vertical = 8.dp),
                    enabled = !isLoading,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.secondary
                    )
                ) {
                    Text("Sync to Cloud")
                }
            }
        }
        
        if (statusMessage.isNotEmpty()) {
            Card(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 16.dp),
                colors = CardDefaults.cardColors(
                    containerColor = MaterialTheme.colorScheme.secondaryContainer
                )
            ) {
                Text(
                    text = statusMessage,
                    modifier = Modifier.padding(16.dp),
                    style = MaterialTheme.typography.bodyMedium
                )
            }
        }
        
        if (isLoading) {
            CircularProgressIndicator(
                modifier = Modifier.padding(16.dp)
            )
        }
        
        if (items.isNotEmpty()) {
            Text(
                text = "Items to sync:",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.padding(vertical = 8.dp)
            )
            
            LazyColumn(
                modifier = Modifier.fillMaxWidth()
            ) {
                items(items) { item ->
                    DataItem(item)
                }
            }
        }
    }
}

@Composable
fun DataItem(data: UserData) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 4.dp),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surfaceVariant
        )
    ) {
        Column(
            modifier = Modifier.padding(12.dp)
        ) {
            Text(
                text = data.name,
                style = MaterialTheme.typography.bodyLarge
            )
            Text(
                text = data.phoneNumber,
                style = MaterialTheme.typography.bodyMedium,
                color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}
