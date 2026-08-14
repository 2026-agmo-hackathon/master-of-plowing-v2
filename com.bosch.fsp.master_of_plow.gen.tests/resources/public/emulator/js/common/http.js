//Wrapper for http methods.
//Provide wrapper methods for GET,POST,PUT and DELETE.
mockCloud.factory('http', function($http) {
	
	var lastHttpTime = 0 ;
		
	function setLastHttpTime(){
		lastHttpTime = Date.now();
	};
	
	var httpHeaders ={
			 'Content-Type': 'application/json; charset=utf-8',
	         
	}
	
	var httpManager = {};
	//GET method, expecting an object as parameter in below format
	// {url: '______'}
	httpManager.get = function(param) {
		setLastHttpTime();
		var cache = false;
		if(param.cache === true){
			cache = true;
		}
		
		return $http({
			method : 'GET',
			headers: httpHeaders,
			url : param.url,
			cache : cache
		});
	};
	
	//POST method, expecting an object as parameter in below format
	// {url: '______',data:{________}}
	httpManager.post = function(param) {
		setLastHttpTime();
	
		return $http({
			method : 'POST',
			headers: httpHeaders,
			url : param.url,
			data : param.data
		});
		
	}
	
	//PUT method, expecting an object as parameter in below format
	// {url: '______',data:{________}}
	httpManager.put = function(param) {
		setLastHttpTime();
	
		return $http({
			method : 'PUT',
			headers: httpHeaders,
			url : param.url,
			data : param.data
		});
	}
	
	//DELETE method, expecting an object as parameter in below format
	// {url: '______',data:{________}}
	httpManager.remove = function(param) {
		setLastHttpTime();
		
		return $http({
			method : 'DELETE',
			headers: httpHeaders,
			url : param.url,
			data : param.data
		});
	}
	//ping the server for availability
	httpManager.ping = function(param) {
		return $http({
			method : 'GET',
			headers: httpHeaders,
			url : param.url,
			cache : false,
			timeout : 20000
		});
	};
	
	// This method returns when the last web-service 
	// call gone to back-end.
	httpManager.getLastHttpTime = function(){
		return lastHttpTime ;
	};
	
	httpManager.removeUserHeader = function()
	{
		delete $http.defaults.headers.common.user;
	};
	
	return httpManager ;
});