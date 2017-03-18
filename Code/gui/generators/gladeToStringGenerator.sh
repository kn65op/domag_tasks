#!/bin/bash

SOURCE_FILE=$1
GENERATED_FILE=$2

echo '#pragma once' > $GENERATED_FILE
echo 'const std::string gladeXml = R"(' >> $GENERATED_FILE
cat $SOURCE_FILE >> $GENERATED_FILE
echo ')";' >> $GENERATED_FILE
