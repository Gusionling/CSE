<%@page import="java.util.Enumeration"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Validation</title>
</head>
<body>
	
	<%
	Enumeration e = request.getParameterNames();
	while(e.hasMoreElements()){
		String name = (String)e.nextElement();
		out.print(name + ": " + request.getParameter(name)+"<br>");
	}
	%>
</body>
</html>