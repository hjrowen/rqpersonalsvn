#!/usr/bin/perl 

#在使用切词的人工控制功能之前，请先用checkworddictman.pl脚本来判断一下你的输入是否是合理合法的。
#
#输入要求如下:
#(1)不能有空的term或者实例片段
#(2)新词在前，实例片段在后
#(3)实例片段的切分结果必须超过1个，如果只有一个那是新词。
#(4)切分结果组合到一起还是实例片段�
#(5)第一行是版本号
#(6)已经添加过的不允许重复添加
#

if(!open(fp, "worddict.man"))
{
   die "控制文件名字必须是worddict.man，请检查!"."\n";
}

my %hash;
my $line = <fp>;
if($line !~ /^ver:/)
{
   die "第一行必须是版本号，请检查!"."\n";
}

my $Nflag = 0;#-Nflag如果是1，则说明新词已经添加完成。
while($line = <fp>)
{
   chomp($line);
   my @array = split(/ /, $line);
   my @temp = split(/\[/, $array[1]);
   my @word = split(/\]/, $temp[1]);
   if($word[0] eq "")
   {
	   print "不能添加空的term"."\n";
	   exit;
   }
   if(exists $hash{$word[0]})
   {
	  print "已经添加过这个词,请去除".":".$word[0]."\n";
	  exit;
   }
   $hash{$word[0]} = 1;

   @temp = split(/\[/, $array[2]);
   @temp1 = split(/\]/, $temp[1]);
   my @fragment = split(/\(/, $temp1[0]);
   my @jointfragment = 0;
   for(my $i = 0; $i < @fragment - 1; $i++)
   {   
       @temp = split(/\)/, $fragment[$i+1]);
       $jointfragment[$i] = $temp[0];
   }
   my $str = join("", @jointfragment);
   if($word[0] ne $str)
   {
	   print "error !\n";
	   print "片段的组合应该和添加的term相同"."\n";
	   print "term:".$word[0]."\t"."fragment:".$str."\n";
	   exit;
   }
   if($array[0] eq "-N")
   {
       if($Nflag == 1)
       {
            print "新词的添加(-N)应该在实例片段(-M)添加之前，请检查!"."\n";
	    exit;
       }
       if(@array != 6)
       {
	       print "添加的新词格式不对"."\t".$line."\n";
	       print "添加新词格式如下:-N [newword] [0(basic1)1(basic2)..] [0(subphrase1)1(subphrase2)] prop weight"."\n";
	       exit;
       }
   }
   elsif($array[0] eq "-M")
   {
	   $Nflag = 1;
	   if(@array != 4)
	   {
		   print "添加的实例片段格式不对"."\t".$line."\n";
		   print "添加实例片段格式如下:-M [seg fragment] [0(seg1)1(seg2)...] -M"."\n";
		   exit;
	   }
	   
   }
}

print "你成功的通过了检查，可以添加该配置文件了，good luck :-)\n";
