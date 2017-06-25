#!/bin/bash
print_alarm(){
var_time=`systemctl list-timers --user | grep alarm.timer | awk {'print $3'} | cut -c -5`
if [[ -n $var_time ]]; then
  echo -ne "\x03a:$var_time"
fi
}

print_pac(){
var_pac=`pacman -Qu | wc -l`
if (( $var_pac > 9 )); then
  echo -ne "\x04#:$var_pac"
fi
}

print_mes(){
var_mail=`cat /tmp/dwm/var_mail`
if [[ -n $var_mail && $var_mail != 0 ]]; then
  echo -ne "\x04@:$var_mail"
fi
}

print_key(){
if [[ `timeout 1 xset -q | grep "Num Lock" | awk {'print $8'}` == off ]]; then
  echo -ne "\x06num:off"
fi
if [[ `timeout 1 xset -q | grep "Caps Lock" | awk {'print $4'}` == on ]]; then
  echo -ne "\x06caps:on"
fi
}

print_mus(){
_title="`timeout 1 mpc current -f %title% | sed -r 's/(.{,40}).*/\1/' `"  #cut иногда некорректно работает с utf
_artist="`timeout 1 mpc current -f %artist% | sed -r 's/(.{,40}).*/\1/' `"
_file="`timeout 1 mpc current -f %file% | sed -r 's/(.{,40}).*/\1/' `"
if [[ -n `mpc | grep playing` ]];then
  if [[ -n $_artist ]];then
    echo -ne "\x07$_artist - $_title"
  elif [[ -z $_title && -z $_srtist ]]; then
    echo -ne "\x07$_file"
  else
    echo -ne "\x07$_title"
  fi
fi
}

#print_load(){
#AVG=(`cat /proc/loadavg`);
#for i in $(seq 0 2); do
#  if [[ ${AVG[$i]} < 1 ]];then
#    echo -ne "\x02.";
#  elif [[ ${AVG[$i]} > 1 && ${AVG[$i]} < 2 ]];then
#    echo -ne "\x04.";
#  else
#    echo -ne "\x05.";
#  fi
#done
#}

#print_temp(){
#_temp=`cat /sys/class/thermal/thermal_zone0/temp | cut -c -2`
#if (( $_temp >= 75 )); then
#  echo -ne "\x06$_temp°C";
#fi
#}

print_bri(){
_bri=$(echo `cat /sys/class/backlight/intel_backlight/brightness`/9.76 | bc)
if (( $_bri <= "95" )); then
  echo -ne "\x03bri:$_bri"
fi;
}

print_vol(){
_vol=`timeout 1 ponymix get-volume`
if (( $_vol != 100 )); then
  echo -ne "\x03vol:$_vol"
fi
}

print_bat(){
BAT=(`cat /sys/class/power_supply/BAT0/uevent | sed "s/POWER.*=//"`)
if [ ${BAT[1]} == "Discharging" ]; then
  if (( ${BAT[11]} >= 25 )); then
    echo -ne "\x03bat:${BAT[11]}"
  elif (( ${BAT[11]} < 25 )); then
    echo -ne  "\x06bat:${BAT[11]}%";
  fi
else
  if ((  ${BAT[11]} != 100 )); then
    echo -ne "\x03bat:${BAT[11]}*"
  fi
fi
}

print_net(){
lan_dev=(`ls /sys/class/net/`)
lan_st=(`cat /sys/class/net/*/operstate`)
for i in $(seq 0 ${#lan_dev[@]}); do
  if [[ ${lan_st[$i]} == "up" ]]; then
    lan_ip=`ip addr show dev ${lan_dev[$i]} | grep 'inet ' | awk '{print $2}' | grep -o '^[^/]*' | head -1`
    if [[ -n $lan_ip ]]; then
      if [[ -z /tmp/dwm/var_ip ]];then
        echo -ne "\x05`echo ${lan_dev[$i]} | cut -c1`:$lan_ip"
      else
        echo -ne "\x02`echo ${lan_dev[$i]} | cut -c1`:$lan_ip"
      fi
    fi
  fi
done
}

print_date(){
  echo -ne "\x03`date "+%H:%M:%S"`"
}

print_trans(){
rt_c="timeout 10 xmlrpc rt.loc d.multicall "main""
if [[ -n `$rt_c "d.get_state=" | grep 'integer: 1'` ]]; then
  _up_tr=(`$rt_c "d.get_up_rate=" | grep integer: | awk '{print $5}'`)
  _down_tr=(`$rt_c "d.get_down_rate=" | grep integer: | awk '{print $5}'`)
  for i in $(seq 0 `echo ${_down_tr[*]} | wc -w`); do
    if [[ -n ${_up_tr[$i]} ]]; then
      let up_tr=$up_tr+${_up_tr[$i]}
      let down_tr=$down_tr+${_down_tr[$i]}
    fi
  done
  if (( $down_tr > $up_tr )); then
    echo -ne "\x08d:`echo $down_tr/1000 | bc`"
  else
    echo -ne "\x08u:`echo $up_tr/1000 | bc`"
  fi
fi
}

print_lang(){
  echo -ne "\x03`timeout 1 skb -1 | tr '[:upper:]' '[:lower:]' | cut -c -2`"
}

while true; do
  xsetroot -name "$(print_key)$(print_mus)$(print_trans)$(print_pac)\
$(print_alarm)$(print_bri)$(print_vol)$(print_bat)$(print_net)$(print_date)$(print_lang)"
  sleep 1
done
