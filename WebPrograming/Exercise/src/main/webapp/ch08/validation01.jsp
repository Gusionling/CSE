<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<title>Validation</title>
</head>
<script type="text/javascript">
	function check(){
		if(document.frm.value==""){
			alert("아이디를 입력해주세요")
			return false;
		}
		else if(document.frm.value==""){
			alert("비밀번호를 입력해주세요")
			return false;
		}
		if((document.frm.passwd.value).search(document.frm.id.value) > -1){
			alert("비밀번호는 ID를 포함할 수 없습니다.");
			return false;
		}
		return true;
		
	}


</script>
<body>
	<form action="validation01_process.jsp" name="frm">
		<p> 아이디 : <input type="text" name="id">
		<p> 비밀번호 : <input type="text" name="passwd">
		<input type="submit" value="전송" onclick="check()">
	</form>
	
</body>
</html>