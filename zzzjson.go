package zzzJSON

/*
#cgo CFLAGS: -Wall -O3
#define zzz_SHORT_API 0
#include "zzzjson.h"
*/
import (
	"C"
)

type JSONSize C.zzz_SIZE
type JSONType C.zzz_JSONType

const (
	JSONTypeBool   JSONType = 1
	JSONTypeArray  JSONType = 2
	JSONTypeObject JSONType = 3
	JSONTypeString JSONType = 4
	JSONTypeNull   JSONType = 5
	JSONTypeNum    JSONType = 6
)

const (
	zzzTrue  = C.zzz_BOOL(1)
	zzzFalse = C.zzz_BOOL(0)
)

type Allocator struct {
	A *C.struct_zzz_Allocator
}

func NewAllocator() *Allocator {
	var allocator Allocator
	allocator.A = C.zzz_AllocatorNew()
	return &allocator
}

func ReleaseAllocator(allocator *Allocator) {
	C.zzz_AllocatorRelease(allocator.A)
}

type Value struct {
	V *C.struct_zzz_Value
}

func NewValue(allocator *Allocator) *Value {
	var v Value
	v.V = C.zzz_ValueNew(allocator.A)
	return &v
}

func (v *Value) Parse(s string) bool {
	ret := C.zzz_ValueParseFast(v.V, C.CString(s))
	if ret != zzzTrue {
		return false
	}
	return true
}

func (v *Value) Stringify() *string {
	ret := C.zzz_ValueStringify(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetKey() *string {
	ret := C.zzz_ValueGetKey(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetUnEscapeKey() *string {
	ret := C.zzz_ValueGetUnEscapeKey(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetStr() *string {
	ret := C.zzz_ValueGetStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetUnEscapeStr() *string {
	ret := C.zzz_ValueGetUnEscapeStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetNum() *string {
	ret := C.zzz_ValueGetNumStr(v.V)
	if ret == nil {
		return nil
	}
	retStr := C.GoString(ret)
	return &retStr
}

func (v *Value) GetBool() *bool {
	ret := C.zzz_ValueGetBool(v.V)
	if ret == nil {
		return nil
	}
	b := false
	if *ret == zzzTrue {
		b = true
	}
	return &b
}

func (v *Value) IsNull() bool {
	ret := C.zzz_ValueIsNull(v.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) Type() *JSONType {
	t := C.zzz_ValueType(v.V)
	if t == nil {
		return nil
	}
	ret := JSONType(*t)
	return &ret
}

func (v *Value) ObjGet(key string) *Value {
	cVal := C.zzz_ValueObjGet(v.V, C.CString(key))
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

func (v *Value) ArrayGet(index JSONSize) *Value {
	cVal := C.zzz_ValueArrayGet(v.V, C.zzz_SIZE(index))
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

func (v *Value) Size() JSONSize {
	return JSONSize(C.zzz_ValueSize(v.V))
}

func (v *Value) Begin() *Value {
	cVal := C.zzz_ValueBegin(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

func (v *Value) Next() *Value {
	cVal := C.zzz_ValueNext(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

func (v *Value) Copy() *Value {
	cVal := C.zzz_ValueCopy(v.V)
	if cVal == nil {
		return nil
	}
	val := Value{V: cVal}
	return &val
}

func (v *Value) Move() bool {
	ret := C.zzz_ValueMove(v.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetNull() {
	C.zzz_ValueSetNull(v.V)
}

func (v *Value) SetBool(b bool) {
	if b == true {
		C.zzz_ValueSetBool(v.V, zzzTrue)
	} else {
		C.zzz_ValueSetBool(v.V, zzzFalse)
	}
}

func (v *Value) SetNumStr(num string) bool {
	ret := C.zzz_ValueSetNumStrFast(v.V, C.CString(num))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetStr(str string) bool {
	ret := C.zzz_ValueSetStrFast(v.V, C.CString(str))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetStrEscape(str string) bool {
	ret := C.zzz_ValueSetStrEscape(v.V, C.CString(str))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetKey(key string) bool {
	ret := C.zzz_ValueSetKeyFast(v.V, C.CString(key))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetKeyEscape(key string) bool {
	ret := C.zzz_ValueSetKeyEscape(v.V, C.CString(key))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetArray() {
	C.zzz_ValueSetArray(v.V)
}

func (v *Value) SetObj() {
	C.zzz_ValueSetObj(v.V)
}

func (v *Value) Set(vv *Value) bool {
	ret := C.zzz_ValueSet(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) SetFast(vv *Value) bool {
	ret := C.zzz_ValueSetFast(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ObjAdd(vv *Value) bool {
	ret := C.zzz_ValueObjAdd(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ObjAddFast(vv *Value) bool {
	ret := C.zzz_ValueObjAddFast(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ArrayAdd(vv *Value) bool {
	ret := C.zzz_ValueArrayAdd(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ArrayAddFast(vv *Value) bool {
	ret := C.zzz_ValueArrayAddFast(v.V, vv.V)
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ObjDel(key string) bool {
	ret := C.zzz_ValueObjDel(v.V, C.CString(key))
	if ret == zzzTrue {
		return true
	}
	return false
}

func (v *Value) ArrayDel(index JSONSize) bool {
	ret := C.zzz_ValueArrayDel(v.V, C.zzz_SIZE(index))
	if ret == zzzTrue {
		return true
	}
	return false
}
