<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.util.Date, java.lang.Math"%>
<html>
<head>
<title>Directive Tag</title>
</head>
<body>
	<br>현재 날짜 : <%=new java.util.Date() %></br>
	<br>5의 제곱 : <%= Math.pow(5, 2) %></br>
</body>
</html>