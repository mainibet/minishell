#!/bin/bash

cc parse.c -o parse && ./parse "c0 | c1 ; c2 | c3 | c4"
