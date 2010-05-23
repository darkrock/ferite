
// ---------- script properties ----------


var results_location = "results.html";


// ---------- end of script properties ----------


function search_form(jse_Form) {
	if (jse_Form.d.value.length > 0) {
		document.cookie = "d=" + escape(jse_Form.d.value);
		parent.main.location.href = results_location;
	}
}
