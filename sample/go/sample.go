package main

import (
	"fmt"

	"github.com/dacez/zzzJSON"
)

func main() {

	A := zzzJSON.NewAllocator()
	defer zzzJSON.ReleaseAllocator(A)
	V := zzzJSON.NewValue(A)
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
