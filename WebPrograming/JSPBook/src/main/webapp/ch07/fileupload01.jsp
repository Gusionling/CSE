<%@ page contentType="text/html; charset=UTF-8"%>
<html>
<head>
<meta charset="UTF-8">
<title>File Upload</title>
</head>
<body>
	<form name = "fileForm" method = "POST" enctype="multipart/form-data"
	action = "fileupload01_process.jsp">
		<p>이름 : <input type = "text" name="name">
		<p>제목 : <input type = "text" name="subject">
		<p>파일 : <input type = "file" name="filename">
		<p><input type = "submit" name="파일 올리기">
		
	</form>
</body>
</html>