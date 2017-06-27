load_blif "~/mul5.blif"
level_layout
show -g
cluster
select --add 1 338 154 150 340
group -m 1
select -c 1 365 359 244 351 252 185 199 353 347 358 195 357
extract -n
