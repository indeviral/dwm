#!/bin/bash

shopt -s nullglob
declare -A selections

files=("$HOME/.cache/clipboard.$USER/"*)

ordered_selections=()
for (( i=${#files[@]}-1; i>=0; i-- )); do
  file=${files[$i]}
  first_line=$(sed -n '/./{p;q}' "$file")
  lines=$(wc -l < "$file")
  if (( $lines > 1 )); then
    first_line+=" [$lines lines]"
  else
    first_line+=" "
  fi
  ordered_selections+=("$first_line")
  selections[$first_line]=$file
done
chosen_line=$(printf '%s\n' "${ordered_selections[@]}" | dmenu "$@")

[[ $chosen_line ]] || exit 1

for selection in clipboard primary; do
    xsel -i --"$selection" < "${selections[$chosen_line]}"
done

