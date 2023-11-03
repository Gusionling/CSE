<%@ page contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Action Tag</title>
</head>
<body>
	<jsp:useBean id="person" class="ch04.com.dao.Person" scope="request"/>
	<jsp:setProperty name="person" property="id" value="20231233"/>
	<jsp:setProperty name="person" property="name" value="웹프"/>
	
	
	<p> 아이디 : <% out.println(person.getId()); %>
	<p> 이 름 : <% out.println(person.getName());  %>
	
</body>
</html>