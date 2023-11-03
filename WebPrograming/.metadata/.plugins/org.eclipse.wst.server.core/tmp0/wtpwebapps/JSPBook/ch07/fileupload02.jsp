<%@ page contentType="text/html; charset=UTF-8"%>
<html>
<head>
<meta charset="UTF-8">
<title>File Upload</title>
</head>
<body>
	<form name = "fileForm" method = "POST" enctype="multipart/form-data"
	action = "fileupload02_process.jsp">
		<p>이름1 : <input type = "text" name="name1">
		<p>제목1 : <input type = "text" name="subject1">
		<p>파일1 : <input type = "file" name="filename1">
		
		<p>이름2 : <input type = "text" name="name2">
		<p>제목2 : <input type = "text" name="subject2">
		<p>파일2 : <input type = "file" name="filename2">
		
		<p>이름3 : <input type = "text" name="name3">
		<p>제목3 : <input type = "text" name="subject3">
		<p>파일3 : <input type = "file" name="filename3">
		<p><input type = "submit" name="파일 올리기">
		
	</form>
</body>
</html>