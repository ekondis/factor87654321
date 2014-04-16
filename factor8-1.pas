Program factor8to1;

uses sysutils;{DateUtils;}

var
	i, j:integer;
	time_start, time_end: TTimeStamp;
	exec_time: comp;

begin
	writeln('Searching for two factors in range [1,15000] whose multiple is 87654321');
	time_start := DateTimeToTimeStamp(Now);
	for i:=1 to 15000 do
		for j:=i+1 to 15000 do
			if i*j=87654321 then
				writeln('Found! ',i,' ',j);
	time_end := DateTimeToTimeStamp(Now);
	exec_time := TimeStampToMSecs(time_end)-TimeStampToMSecs(time_start);
	writeln('kernel execution time = ', exec_time:8:2, ' msecs');
end.

