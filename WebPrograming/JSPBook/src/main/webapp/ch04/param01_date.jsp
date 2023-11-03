<%@ page contentType="text/html; charset=UTF-8"%>
<html>
<head>
<meta charset="UTF-8">
<title>Action Tag</title>
</head>
<body>
	<p>아이디 : <%=request.getParameter("id") %>
		<%
			String name = request.getParameter("name");
		%>
	<p>이름 : <%=java.net.URLDecoder.decode(name)%>
</body>
</html>