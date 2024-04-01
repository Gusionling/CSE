<%@ page contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Action Tag</title>
</head>
<body>
	<jsp:useBean id="person" class="ch04.com.dao.Person" scope="request"/>
	<jsp:getProperty name="person" property="id" />
	<jsp:getProperty name="person" property="name" />
	
	
</body>
</html>