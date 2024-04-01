<%@page import="dao.BookRepository"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@page import="dto.Book"%>
<%@page import="java.util.ArrayList"%>
<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css">
<title>도서목록</title>
</head>
<body>
	<%@ include file="menu.jsp" %>
	<div class = "jumbotron">
		<div class = "container">
			<h1 class = "display-3">도서 목록</h1>
		</div>
	</div>
	<%
		BookRepository dao = BookRepository.getInstance();
		ArrayList<Book> listOfBooks = dao.getAllProducts();
	%>
	<div class="container">
		<div class="row">
			<jsp:useBean id="books" class="dao.BookRepository"/>
			<%
				for (int i=0; i<listOfBooks.size(); i++) {
					Book b = listOfBooks.get(i);
			%>
			
			<div class="col-md-10">
				<h4><%="["+b.getCategory()+"] " + b.getName()%></h4>
				<br>
				<P><%=b.getBookId() %>
				<p><%=b.getDescription()%>
				<p><%=b.getAuthor() + " | " + b.getPublisher() + " | " + b.getUnitPrice() + "원"%>
				<hr style="border-style: dotted;" />
			</div>
			<div class="col-md-2">
				<p><a href="./product.jsp?id=<%=b.getBookId()%>" class="btn btn-secondary">상세 정보 &raquo;</a></p>
			</div>
			<%
				}
			%>
		</div>
	</div>
	<%@ include file="footer.jsp" %>
</body>
</html>