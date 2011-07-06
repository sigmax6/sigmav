package main

import (
    "os"
    "flag"
)

var omitNewline = flag.Bool("n",false,"No new line")

const (
     Space = " "
     Newline = "\n"
)

func main () {
    flag.Parse()       //Scan the arg list and set flag
    var s string = ""
    for i:=0 ; i<flag.NArg() ; i++ {
        if i > 0 {
            s += Space
        }
        s += flag.Arg(i)
    }
    if !*omitNewline {
        s += Newline
    }
    os.Stdout.WriteString(s)
}

