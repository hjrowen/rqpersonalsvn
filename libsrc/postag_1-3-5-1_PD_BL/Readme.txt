Postag Project  
Version 1.2.4   
Baidu@2007~2009
David Dai
Updated on 2009.4

1.代码说明
  本版本开始采用简单清晰的代码风格，约定如下：
  a) 函数名小写，多个单词之间用下划线“_”连接，内部函数以下划线开头。
  b) 变量名如果是单个单词小写，不以下划线开头；多个单词的话从第二个单词开始每个单词首字母大写，如pFullPathName，以区别函数命名。
  c) 自定义类型名以“_t”结尾，自定义结构内的成员变量小写，多个单词之间用下划线连接。
  d) 文件名小写，但是采用.cpp后缀，表示采用非面向对象的C++程序。  
  e) 只承担接口作用的头文件以i开头，以区别于内部头文件。

2.文件夹说明    
  dict - 辞典原始数据以及辞典索引数据目录
  
  src - 源码文件目录
  include - 头文件目录
  data - 存放二进制词典文件
  test - 单元测试源码文件，以及特定的应用程序源文件
  
  lib - make生成的库文件目录  
  bin - make生成的程序，脚本，测试数
  doc - 用于存放自动生成的代码文档
    
  output - 临时输出目录,仅供SCM使用，make clean后会删除
  
3.下一步计划
  Postag:
  1) 针对特殊的句型，加入规则化方法进行标注，这个也将以下面所说的结构分析为基础来进行。
  Chunk Parsing:  
  1)组块分析将进一步演变为结构分析，针对结构化query进行识别。
    结构型query case：
    **英语怎么说
    从**到** **票
    **字繁体怎么写        
    V(做，干) ** 怎么样
    ** 人多吗
    站点+资源
    日成念什么
    

Look out：
  1) 接口头文件(i开头的文件)中的数据结构以及函数声明均不能轻易修改！
  2) 接口数据结构token_t类型在wordseg中也有相同的定义，postag中的定义必须和wordseg的定义完全一致！
  
Change log: 
V1.1.0
  1) Readme文件都进行了更新；加入自动生成代码文档的脚本，并统一代码注释风格。
  2) 增加chunk parsing模块，具体添加的代码均通过新的文件，尽量不修改已有postag代码，以方便将来合并！
  3) 原来在tagpos.cpp中的常量数组均移到了tagdict.cpp中，然后通过extern在tagdict.h中进行了声明。
  4) term权重计算以前只参照chunk的head，后调整为参照chunk中所有term来计算。

Change log:
V1.2.4
  1) 加入短串主干分析模块。
  2) 标注模块中加入词典结构外置的接口。
  3) 如果标注返回的结果token_t的weight与上OUT_OF_TAG(0x800000)，即最高位为1
     (注意token_t的weight仅使用了24位)，则表示此token对应的词语不在词性标记词典中。
     使用此字段的用户需要注意这个变化。

Change log:
V1.2.5  
  1) 引入动态term重要性计算，并以其为基础进行大致分层。
  2) 引入了知识规则：如上下位，专名+属性，资源+站点等，用来细化分层。

Change log:
V1.3.1
  1) 引用的动态term重要性模块升级，主要解决单字重要性不合理问题，高频term重要性偏高的问题。[done，已在wordrank模块处理]
  2) 解决中英文term混合query中英文term权重偏低问题。[done，已在wordrank模块处理]
  3) 数字开头及数字结尾；版本号term初始化为最低权重! [done，已在wordrank模块处理]     
  4) 去除了词性组合规则。[done]
  5) 如果主干term个数小于2个，则根据term重要性从非主干层拉会一个重要性最大的，以前是优先拉回标记为限定的属性词。[done，已在queryanalysis模块处理]

Change log:
V1.3.2
  1) 调整地名及属性词策略，使其不直接进入省略层，而是根据term重要性计算来决策。
  
Change log:
V1.3.4
  1) 去除stopwords策略。[done]
  2) 纯英文query不进行分层省略处理。[done]

Known problem:
  1) 没有进行分层处理的query，其term重要性也没有取出，这个后续需要修改。
  2) 句子型query需要辅助以结构分析才能有效提取主干，并进行准确的分层。 
  3) 属性词策略问题，如“显卡驱动”被拆分后，“驱动”命中属性词进入中间层，
     而“显卡”不会，，实际上“驱动”比“显卡”term重要性高不少。类似的还有“网上办公”等。
  

分层平均词语长度变化(随机)： 
new:    
0.135455        0.660139        2.16901
9804

0.164117        0.581395        2.22776
9804
