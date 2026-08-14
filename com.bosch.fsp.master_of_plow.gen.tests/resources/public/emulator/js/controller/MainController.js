mockCloud.controller('MainController', function($state, service){
    console.log('MainController');

    var vm=this;
    vm.context={}

    $state.go('main.dashboard');

    vm.gotodashboard = function (){
        vm.context.selected=false;
        $state.go('main.dashboard');
    };

    vm.pushtofeature = function (){
        vm.context.selected=true;
        $state.go('main.download');
    };
});

    
    
