#!/bin/bash

code=1234
mail="mon.email@uha.fr"
nbRestant=3
s=1

while [ "$s" -eq 1 ]
do
  if [ $nbRestant -eq 0 ]
  then
    echo "Envoyez une demande de PIN à $mail"
    break
  fi
  echo "Votre code"
  read tmpCode
  if [ $tmpCode -eq $code ]
  then
    echo "Vous êtes connecté"
    
    s=0
  else
    let "nbRestant-=1"
    echo "Vous vous êtes trompé, il vous reste $nbRestant essais."
  fi
done