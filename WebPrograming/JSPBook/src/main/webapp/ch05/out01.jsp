<%@ page contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Implicit Object</title>
</head>
<body>
	<%
		out.println("오늘의 날짜 및 시각 " + "<br>");
		out.println(java.util.Calendar.getInstance().getTime());
	
	%>

</body>
</html>