# sldms

sldms is a stable and effective tool for calculating the overlapping regions of sequences。

sldms runs in internal memory and the results are written to disk.

## Compilation and installation

sldms will compile in systems with a standard C compiler (like gcc) and make. 

```sh
git clone https://github.com/Dongliang-You/sldms.git
cd sldms
make 
```
Issuing these commands will build executables sldms and sldms-64.

For inputs larger than **2GB**, **sldms-64_** must be used.

To enable support to compressed files, zlib is required.  If zlib is
not installed in you system, build with option ``make GZ=0``.

## Execution

```sh
./sldms INPUT [options]
```
where INPUT is a single file with a string collection.

### options:

```sh
--corr                choose Exact matching version
--raw                 choose Fault-tolerant matching version
--time                output time (seconds)
--verbose             verbose output
--topk                get top-k answer
--tolerance	      only fault-tolerant matching version need. Represents the maximum cut length allowed for sequencing data.
--help                help message
```

## Input files 

- File types (text, fasta or fastq) will be selected by extensions:
  _.txt_, _.fasta_ (or _.fa_, _.fna._) and _.fastq_ (or _.fq_).

- In _txt_ files, each line is taken as a strings in the collection.
  In _fasta_ and _fastq_ files, each sequence is taken as a string in the
  collection.

- sldms supports the ASCII alphabet, but values _0_ and _255_ are
  reserved and must not occur in the input.

- IUPAC symbols and 'N' are not handled as special symbols in _fasta_
  or _fastq_ files.

## Output files 
- Output files are written by default in the current directory. filename is tempout.txt.

## quick test

To run a test with all strings from ``dataset/example.txt``, type:

```sh
./sldms dataset/example.txt --corr --topk=10
```

```sh
## sldms ##
### output format

  if topk=10
  
- corr-output
  Each row corresponds to one read, and there are at most  twenty-one digits. 
  The first digit is the sequence number of read, and the following two digits are in a group, 
  and respectively are the sequence number and length of overlap of read with the best overlap information.

- raw-output
  Each line corresponds to one read, and there are at most forty-five digits.
  The first digit is the sequence number of the read, 
  The next four numbers represent the information of the best answer. 
  The first two numbers represent the serial number and overlap length of the read, 
  and the last two numbers represent the size to be trimmed for splicing the answer-read and the current-read. 
  The former is the size to be trimmed for the head of the current-read, 
  and the latter is the size to be trimmed for the tail of the answer-read.
  then there is information about the top ten best answers.

## authors
* [Dongliang You](https://github.com/Dongliang-You)
```
## To other software developers:

If you want to know the operation details of sldms, you can search the papers on sldms. The papers provide detailed design ideas and implementation details. You can read the papers together with the code.

If you meet one of the following conditions:

1. You are preparing to develop other downstream software in the genome assembly software based on OLC (overlap layout consensus) algorithm, and sldms can just be your upstream software.

2. You are developing a complete genome assembly software, and you want to take sldms as a part of your software.

For the above two cases, if you do not need to input the sldms results to the disk, but need the sldms results to be used as data in your software, you can choose to modify the 251 / 258 / 259 / 544 / 546 / 549 lines of code in lib / until. C, use an array to receive these data and use them in return values or other forms, Use this as the data input part or intermediate result part of your software.

If you have any questions about this software, please send an email to the author. If the author sees your email, he will provide you with the corresponding answers. Mail: 627542498@nefu.edu.cn
