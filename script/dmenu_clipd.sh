#!/bin/bash

cache_dir=$HOME/.cache/clipboard.$USER/
mkdir -p -m0700 "$cache_dir"

declare -A last_data
declare -A last_filename
while sleep "${CLIPMENUD_SLEEP:-0.5}"; do
  for selection in clipboard primary; do
    data=$(timeout 5 xsel -o --"$selection"; printf x)
      if [[ $selection != primary ]]; then
        timeout 1 xsel -o --"$selection" | xsel -i --"$selection"
      fi
    data=${data%x}
    [[ $data == *[^[:blank:]]* ]] || continue
    [[ ${last_data[$selection]} == "$data" ]] && continue
    possible_partial=${last_data[$selection]}
    if [[ $possible_partial && $data == "$possible_partial"* ]]; then
      rm -- "${last_filename[$selection]}"
    fi
    if (( `ls $cache_dir | wc -l` > 90 )); then
      rm -f $cache_dir/`ls -1rt $cache_dir | head -1`
    fi
    filename="$cache_dir/$(LC_ALL=C date +%F-%H-%M-%S)"
    last_data[$selection]=$data
    last_filename[$selection]=$filename
    printf '%s' "$data" > "$filename"
  done
done

