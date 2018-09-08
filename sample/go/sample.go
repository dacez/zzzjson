package main

import (
	"fmt"

	"github.com/dacez/zzzjson"
)

func main() {

	A := zzzjson.NewAllocator()
	defer A.ReleaseAllocator()
	V := zzzjson.NewValue(A)
	json := `[true,false]`

	ret := V.Parse(json)
	if ret != true {
		fmt.Println("Parse Error!")
		return
	}

	V1 := V.ArrayGet(1)
	ret = V1.SetStr("1234..456")
	if ret != true {
		fmt.Println("SetNumStr Error!")
		return
	}
	out := V.Stringify()
	if out != nil {
		fmt.Println(*out)
	}

	return
}
