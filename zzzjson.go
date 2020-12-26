package zzzjson

/*
#cgo CFLAGS: -Wall -O3
#define zj_SHORT_API 0
#include "zzzjson.h"
*/
import (
	"C"
)

/*
JSONSize Size for zzzJSON
*/
type JSONSize C.zj_Size

/*
JSONType Type for JSON
*/
type JSONType C.zj_JSONType

/*
6 types for JSON
include bool array object string null and number
*/
const (
	JSONTypeBool   JSONType = 1
	JSONTypeArray  JSONType = 2
	JSONTypeObject JSONType = 3
	JSONTypeString JSONType = 4
	JSONTypeNull   JSONType = 5
	JSONTypeNumber JSONType = 6
)



/*
Allocator Allocate memory for zzzJSON
*/
type Allocator struct {
	A *C.struct_zj_Allocator
}

/*
NewAllocator Create an allocator
*/
func NewAllocator() *Allocator {
	var allocator Allocator
	allocator.A = C.zj_NewAllocator()
	return &allocator
}

/*
ReleaseAllocator free an allocator
*/
func (allocator *Allocator) ReleaseAllocator() {
	C.zj_AllocatorRelease(allocator.A)
}

/*
Value JSON Value, it can be one of 6 jsontypes
*/
type Value struct {
	V *C.struct_zj_Value
}

/*
NewValue Create a JSON Value using allocator
*/
func NewValue(allocator *Allocator) *Value {
	var v Value
	v.V = C.zj_NewValue(allocator.A)
	return &v
}

/*
Parse Parse JSON text to value
*/
func (v *Value) Parse(s string) bool {
	ret := C.zj_ParseFast(v.V, C.CString(s))
	if ret != true {
		return false
	}
	return true
}

/*
Stringify Stringify Vaue to JSON text
*/
func (v *Value) Stringify() *string {
	ret := C.zj_Stringify(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetKey Get key of value
*/
func (v *Value) GetKey() *string {
	ret := C.zj_GetKey(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetUnEscapeKey Get key of value and unescape key
*/
func (v *Value) GetUnEscapeKey() *string {
	ret := C.zj_GetUnEscapeKey(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetStr Get string of value
*/
func (v *Value) GetStr() *string {
	ret := C.zj_GetStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetUnEscapeStr Get string of value and unescape
*/
func (v *Value) GetUnEscapeStr() *string {
	ret := C.zj_GetUnEscapeStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetNum Get number of value
*/
func (v *Value) GetNum() *string {
	ret := C.zj_GetNumStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

/*
GetBool Get bool of value
*/
func (v *Value) GetBool() *bool {
	ret := C.zj_GetBool(v.V)
	if ret == nil {
		return nil
	}
	b := false
	if *ret == true {
		b = true
	}
	return &b
}

/*
IsNull check value is null or not
*/
func (v *Value) IsNull() bool {
	ret := C.zj_IsNull(v.V)
	if ret == true {
		return true
	}
	return false
}

/*
Type get type of value
*/
func (v *Value) Type() *JSONType {
	t := C.zj_Type(v.V)
	if t == nil {
		return nil
	}
	ret := JSONType(*t)
	return &ret
}

/*
ObjGet get value of object where key == key
*/
func (v *Value) ObjGet(key string) *Value {
	cVal := C.zj_ObjGet(v.V, C.CString(key))
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

/*
ArrayGet get value of array where index == index
*/
func (v *Value) ArrayGet(index JSONSize) *Value {
	cVal := C.zj_ArrayGet(v.V, C.zj_Size(index))
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

/*
Size get size of value(array/object)
*/
func (v *Value) Size() JSONSize {
	return JSONSize(C.zj_SizeOf(v.V))
}

/*
Begin get first child of value(array/object)
*/
func (v *Value) Begin() *Value {
	cVal := C.zj_Begin(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

/*
Next get next child from current child
*/
func (v *Value) Next() *Value {
	cVal := C.zj_Next(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

/*
Copy copy an value
*/
func (v *Value) Copy() *Value {
	cVal := C.zj_Copy(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

/*
Move move an value
*/
func (v *Value) Move() bool {
	ret := C.zj_Move(v.V)
	if ret == true {
		return true
	}
	return false
}

/*
SetNull set value null
*/
func (v *Value) SetNull() {
	C.zj_SetNull(v.V)
}

/*
SetBool set value bool
*/
func (v *Value) SetBool(b bool) {
	if b == true {
		C.zj_SetBool(v.V, true)
	} else {
		C.zj_SetBool(v.V, false)
	}
}

/*
SetNumStr set number to value
*/
func (v *Value) SetNumStr(num string) bool {
	ret := C.zj_SetNumStrFast(v.V, C.CString(num))
	if ret == true {
		return true
	}
	return false
}

/*
SetStr set string to value
*/
func (v *Value) SetStr(str string) bool {
	ret := C.zj_SetStrFast(v.V, C.CString(str))
	if ret == true {
		return true
	}
	return false
}

/*
SetStrEscape set string to value and escape, string can contain \n\r...
*/
func (v *Value) SetStrEscape(str string) bool {
	ret := C.zj_SetStrEscape(v.V, C.CString(str))
	if ret == true {
		return true
	}
	return false
}

/*
SetKey set key to value
*/
func (v *Value) SetKey(key string) bool {
	ret := C.zj_SetKeyFast(v.V, C.CString(key))
	if ret == true {
		return true
	}
	return false
}

/*
SetKeyEscape set key to value and escape, string can contain \n\r...
*/
func (v *Value) SetKeyEscape(key string) bool {
	ret := C.zj_SetKeyEscape(v.V, C.CString(key))
	if ret == true {
		return true
	}
	return false
}

/*
SetArray set value to array
*/
func (v *Value) SetArray() {
	C.zj_SetArray(v.V)
}

/*
SetObj set value to object
*/
func (v *Value) SetObj() {
	C.zj_SetObj(v.V)
}

/*
Set set an value by copy
*/
func (v *Value) Set(vv *Value) bool {
	ret := C.zj_Set(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
SetFast set an value by move
*/
func (v *Value) SetFast(vv *Value) bool {
	ret := C.zj_SetFast(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
ObjAdd add an value to object by copy
*/
func (v *Value) ObjAdd(vv *Value) bool {
	ret := C.zj_ObjAdd(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
ObjAddFast add an value to object by move
*/
func (v *Value) ObjAddFast(vv *Value) bool {
	ret := C.zj_ObjAddFast(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
ArrayAdd add an value to array by copy
*/
func (v *Value) ArrayAdd(vv *Value) bool {
	ret := C.zj_ArrayAdd(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
ArrayAddFast add an value to array by move
*/
func (v *Value) ArrayAddFast(vv *Value) bool {
	ret := C.zj_ArrayAddFast(v.V, vv.V)
	if ret == true {
		return true
	}
	return false
}

/*
ObjDel delete an value of object where key == key
*/
func (v *Value) ObjDel(key string) bool {
	ret := C.zj_ObjDel(v.V, C.CString(key))
	if ret == true {
		return true
	}
	return false
}

/*
ArrayDel delete an value of array where index == index
*/
func (v *Value) ArrayDel(index JSONSize) bool {
	ret := C.zj_ArrayDel(v.V, C.zj_SIZE(index))
	if ret == true {
		return true
	}
	return false
}
