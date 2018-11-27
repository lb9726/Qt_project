#LogHandler

A log tool developed by Qt, using qInstallMessageHandler

程序启动时 logFile 为 NULL，初始化 logFile，有可能是同一天打开已经存在的 logFile，所以使用 Append 模式.

logFileCreatedDate is null, 说明日志文件在程序开始时不存在，所以记录下创建时间.

程序运行时检查如果 logFile的创建日期和当前日期不相等，则使用它的创建日期重命名，然后再生成一个新的 log.txt 文件.