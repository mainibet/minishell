#!/bin/bash

cc parse.c -o parse && ./parse "c0 | c1 ; c2 | c3 | c4 ; c5 | c6 | c7"
