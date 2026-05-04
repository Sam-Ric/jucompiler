declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

@.str.int    = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.string = private unnamed_addr constant [3 x i8] c"%s\00"
@.str.true   = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false  = private unnamed_addr constant [6 x i8] c"false\00"

define i32 @factorial(i32 %n_arg) {
entry:
  %n = alloca i32
  store i32 %n_arg, i32* %n
  %2 = load i32, i32* %n
  %3 = add i32 0, 0
  %4 = icmp eq i32 %2, %3
  br i1 %4, label %if.then.1, label %if.else.1
if.then.1:
  %5 = add i32 0, 1
  ret i32 %5
unreachable.6:
  br label %if.end.1
if.else.1:
  br label %if.end.1
if.end.1:
  %7 = load i32, i32* %n
  %8 = load i32, i32* %n
  %9 = add i32 0, 1
  %10 = sub i32 %8, %9
  %11 = call i32 @factorial(i32 %10)
  %12 = mul i32 %7, %11
  ret i32 %12
unreachable.13:
  ret i32 0
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %args.argc = alloca i32
  store i32 %argc, i32* %args.argc
  %args.argv = alloca i8**
  store i8** %argv, i8*** %args.argv
  %argument = alloca i32
  %1 = add i32 0, 7
  store i32 %1, i32* %argument
  %2 = load i32, i32* %argument
  %3 = call i32 @factorial(i32 %2)
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.int, i32 0, i32 0), i32 %3)
  ret i32 0
}

