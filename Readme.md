

This is a toydb, based on Chapter 3 : "Designing Data Intensive Applications" by Martin Kleppmann

### Supported (v0.1):
  - support simple get/put operations
  - write the key-value in the file simpletoy.db 
  - each put appends to the file
  - an in memory index stores the offset of the value
  - the get operation seeks to the offset and reads the value
  - file format : sequence\<entry\>'\n'
  - entry format : `\<keysize\>\<keycontents\>\<valuesize\>\<valuecontents\>`
  - support compaction

### TODO:
  - delete older key values
  - support delete, put tombstone to deleted value
  - support multi segments of db file
  - support SSTable, LSM tree
  - support a B+Tree based implementation, for good sequential read performance
