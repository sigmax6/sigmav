package main

import (
    "os"
    "flag"
)

const (
    Space = " "
    Newline = "\n"
)

var omitNewLine = flag.Bool("n",false,"No new line")

func main(){
    flag.Parse()
    s := ""
    for i := 0 ; i < flag.NArg() ; i++ {
         if (i > 0){
              s += Space
         }
         s += ":"
         s += flag.Arg(i)
    }
    if !*omitNewLine {
        s += Newline
    }
    os.Stdout.WriteString(s)
}


