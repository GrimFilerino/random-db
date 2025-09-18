package database

/*
#cgo CFLAGS: -I../libs
#cgo LDFLAGS: -L../libs -ldb
#include "db.h"
#include <stdlib.h>
*/
import "C"

import (
	"unsafe"
)


type Database interface {
	Create(collection string, data string) int
	Update(collection, id, expectedRev, changedFields string, removedFields []string) int
	Delete(collection, id string) int
	Grep(collection string, limit int) []string
	GrepByID(collection, id string) string

	AddToIndex(collection, indexName, id string) int
	RemoveFromIndex(collection, indexName, id string) int
	GrepByIndex(collection, indexID string, limit int) []string
}

type cDatabase struct{}


func (db *cDatabase) Create (collection string, data string) int {
cCollection := C.CString(collection);
	cData := C.CString(data);

	defer C.free(unsafe.Pointer(cCollection));
	defer C.free(unsafe.Pointer(cData));

	return int(C.reate(cCollection, cData))
}
