class index
{
	api_path = 'http://localhost:8000/api/';
	setting;
	relay;
	constructor()
	{
		var context = this;
		$.ajax(
			this.api_path + 'setting',
			{
				type: 'GET',
				crossDomain: true,
				success: function(data)
				{
					context.setting = data;
					context.redraw();
					console.log(data);
				}
			}
		);
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				crossDomain: true,
				success: function(data)
				{
					context.relay = data;
					context.update_menu();
					console.log(data);
				}
			}
		);
	}
	get_query(name, url = window.location.href)
	{
		name = name.replace(/[\[\]]/g, '\\$&');
		let regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'), results = regex.exec(url);
		if (!results)
			return null;
		if (!results[2])
			return '';
		return decodeURIComponent(results[2].replace(/\+/g, ' '));
	}
	update_menu()
	{
		let relays_menu = document.getElementById('__relays_menu');
		relays_menu.innerHTML = JSON.stringify(this.relay);
	}
	redraw()
	{
		let _page = this.get_query("menu");
		let _html = $('#__main_area').empty();
		_html.append($('<br>'));
		if (_page == 'home')
		{
			_html.append($('h3').text("Home page"));
		}
		else if (_page == 'system')
		{
			_html.append($('h3').text("System page"));
		}
		else
			_html.append($('h3').text("Unknown page"));
	}
}

new index();
