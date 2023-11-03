<%@ page contentType="text/html; charset=UTF-8"%>
<html>
<head>
<meta charset="UTF-8">
<title>File Upload</title>
</head>
<body>
	<form name = "fileForm" method = "POST" enctype="multipart/form-data"
	action = "fileupload03_process.jsp">
		<p>파일 : <input type = "file" name="filename">
		
		<p><input type = "submit" name="파일 올리기">
		
	</form>
</body>
</html>