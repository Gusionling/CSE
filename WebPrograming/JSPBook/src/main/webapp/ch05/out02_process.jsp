<%@ page contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Implicit Object</title>
</head>
<body>
	<%
		request.setCharacterEncoding("utf-8");
		String userid = request.getParameter("id");
		String password = request.getParameter("passwd");
	
	%>
	<p>아 이 디 : <% out.println(userid); %>
	<p>비밀번호 : <% out.println(password); %>
</body>
</html>