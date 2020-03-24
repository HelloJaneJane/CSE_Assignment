use Pokemon

#1
select t.name from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
group by owner_id having count(*)>=3
order by count(*) desc;

#2
select name from Pokemon
where type in (
 select type from (
  select type from Pokemon
  group by type order by count(name) desc, name
  limit 2)
 as tmp
)
order by name;

#3
select name from Pokemon where name like '_o%'
order by name;

#4
select nickname from CatchedPokemon
where level>=50
order by nickname;

#5
select name from Pokemon
where name like '______'
order by name;

#6
select name from Trainer
where hometown='Blue City'
order by name;

#7
select distinct hometown from Trainer
order by hometown;

#8
select avg(cp.level) from CatchedPokemon cp
join Trainer t on cp.owner_id = t.id
where t.name = 'Red';

#9
select nickname from CatchedPokemon
where nickname not like 'T%'
order by nickname;

#10
select nickname from CatchedPokemon
where level>=50 and owner_id>=6
order by nickname;

#11
select id, name from Pokemon
order by id asc;

#12
select nickname from CatchedPokemon
where level<=50
order by level asc;

#13
select p.name, p.id from Pokemon p
join CatchedPokemon cp on p.id = cp.pid
join Trainer t on cp.owner_id = t.id
where t.hometown = 'Sangnok City'
order by p.id asc;

#14
select cp.nickname from CatchedPokemon cp
join Gym g on cp.owner_id = g.leader_id
join Pokemon p on cp.pid = p.id
where p.type = 'Water'
order by cp.nickname asc;

#15
select count(*) from CatchedPokemon cp
join Evolution e on cp.pid = e.before_id;

#16
select count(*) from Pokemon p
where p.type='Water' or p.type='Electric' or p.type='Psychic';

#17
select count(distinct cp.pid) from CatchedPokemon cp
join Trainer t on cp.owner_id = t.id
where t.hometown = 'Sangnok City';

#18
select max(cp.level) from CatchedPokemon cp
join Trainer t on cp.owner_id = t.id
where t.hometown = 'Sangnok City';

#19
select count(distinct p.type) from Pokemon p
join CatchedPokemon cp on p.id = cp.pid
join Gym g on cp.owner_id = g.leader_id
where g.city = 'Sangnok City';

#20
select t.name, count(cp.pid) from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
where t.hometown = 'Sangnok City'
group by t.name
order by count(cp.pid) asc;

#21
select name from Pokemon 
where name like 'a%' or name like 'e%' or name like 'o%'
or name like 'u%' or name like 'i%';

#22
select type, count(id) from Pokemon
group by type order by count(id) asc, type asc;

#23
select distinct t.name from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
where cp.level<=10
order by t.name;

#24
select c.name, avg(cp.level) from City c
join Trainer t on c.name = t.hometown
join CatchedPokemon cp on t.id = cp.owner_id
group by c.name
order by avg(cp.level) asc;

#25
select distinct p.name from Pokemon p
join CatchedPokemon cp on p.id = cp.pid
join Trainer t on cp.owner_id = t.id and t.hometown = 'Sangnok City'
where p.name in (
 select p.name from Pokemon p
 join CatchedPokemon cp on p.id = cp.pid
 join Trainer t on cp.owner_id = t.id and t.hometown = 'Brown City')
order by p.name;

#26
select p.name from Pokemon p
join CatchedPokemon cp on p.id = cp.pid
where cp.nickname like '% %'
order by p.name desc;

#27
select t.name, max(cp.level) from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
group by t.name having count(cp.id)>=4
order by t.name;

#28
select t.name, avg(cp.level) from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
join Pokemon p on cp.pid = p.id
where p.type = 'Normal' or p.type = 'Electric'
group by t.name
order by avg(cp.level) asc;

#29
select p.name, t.name, c.description from Pokemon p
join CatchedPokemon cp on p.id = cp.pid
join Trainer t on cp.owner_id = t.id
join City c on t.hometown = c.name
where p.id = 152 order by cp.level desc;

#30
select p1.id, p1.name, p2.name, p3.name from Pokemon p1
join Evolution e1 on p1.id = e1.before_id
join Pokemon p2 on e1.after_id = p2.id
join Evolution e2 on p2.id = e2.before_id
join Pokemon p3 on e2.after_id = p3.id
order by p1.id asc;

#31
select name from Pokemon
where id>=10 and id<=99
order by name;

#32
select p.name from Pokemon p
where p.id not in
 (select cp.pid from CatchedPokemon cp)
order by p.name;

#33
select sum(cp.level) from CatchedPokemon cp
join Trainer t on cp.owner_id = t.id
where t.name = 'Matis';

#34
select t.name from Trainer t
join Gym g on t.id = g.leader_id
order by t.name;

#35
select type, count(*)/(select count(*) from Pokemon) *100
from Pokemon group by type
order by count(*) desc
limit 1;

#36
select t.name from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
where cp.nickname like 'A%'
order by t.name;

#37
select t.name, sum(cp.level) from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
group by t.id
order by sum(cp.level) desc
limit 1;

#38
select p.name from Evolution e
join Pokemon p on e.after_id = p.id
where e.before_id not in (
 select after_id from Evolution)
order by p.name;

#39
select t.name from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
group by t.name
having count(cp.pid) > count(distinct cp.pid)
order by t.name;

#40
select t.hometown, cp.nickname
from Trainer t
join CatchedPokemon cp on t.id = cp.owner_id
where (t.hometown, cp.level) in (
 select t.hometown, max(cp.level) from Trainer t
 join CatchedPokemon cp on t.id = cp.owner_id
 group by t.hometown)
order by t.hometown;

