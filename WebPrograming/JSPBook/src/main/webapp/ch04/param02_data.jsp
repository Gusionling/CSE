<%@ page contentType="text/html; charset=UTF-8"%>
<html>
<head>
<meta charset="UTF-8">
<title>Action Tag</title>
</head>
<body>
		<%
			String title= request.getParameter("title");
		%>
	<h3><%=java.net.URLDecoder.decode(title) %></h3>
	Today is :<%=request.getParameter("date") %>	
</body>
</html>