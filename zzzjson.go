package main

/*
#cgo CFLAGS: -Wall -O3
#include "zzzjson.h"
*/
import "C"
import "fmt"

func main() {
	fmt.Println("test")
	A := C.NewAllocator()
	V := C.NewValue(A)
	json := "[true, false, true]"
	ret := C.ParseFast(V, C.CString(json))
	fmt.Println("ret", ret)
	json = C.GoString(C.Stringify(V))
	fmt.Println("out", json)
	C.ReleaseAllocator(A)

	return
}
