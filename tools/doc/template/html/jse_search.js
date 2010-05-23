
// Modified by ctr to *only* search the description and keywords
// ---------- script properties ----------

var include_num = 1;
var bold = 1;

// ---------- end of script properties and sites ----------


var cookies = document.cookie;
var p = cookies.indexOf("d=");

if (p != -1) {
    var st = p + 2;
    var en = cookies.indexOf(";", st);
    if (en == -1) {
        en = cookies.length;
    }
    var d = cookies.substring(st, en);
    d = unescape(d);
}
var od = d;
var m = 0;
if (d.charAt(0) == '"' && d.charAt(d.length - 1) == '"') {
    m = 1;
}

var r = new Array();
var co = 0;

if (m == 0) {
    var woin = new Array();
    var w = d.split(" ");
    for (var a = 0; a < w.length; a++) {
        woin[a] = 0;
        if (w[a].charAt(0) == '-') {
            woin[a] = 1;
        }
    }
    for (var a = 0; a < w.length; a++) {
        w[a] = w[a].replace(/^\-|^\+/gi, "");
    }
    a = 0;
    for (var c = 0; c < s.length; c++) {
        pa = 0;
        nh = 0;
        for (var i = 0; i < woin.length; i++) {
            if (woin[i] == 0) {
                nh++;
                var pat = new RegExp(w[i], "i");
                var brokenDownIndex = s[c].split("^");
                var rn = brokenDownIndex[2].search(pat);
                var ro = brokenDownIndex[3].search(pat)
                    if ((rn >= 0) || (ro >= 0)) {
                        pa++;
                    } else {
                        pa = 0;
                    }
            }
            if (woin[i] == 1) {
                var pat = new RegExp(w[i], "i");
                var brokenDownIndex = s[c].split("^");
                var rn = brokenDownIndex[2].search(pat);
                var ro = brokenDownIndex[3].search(pat)
                    if ((rn >= 0) || (ro >= 0)) {
                        pa = 0;
                    }
            }
        }
        if (pa == nh) {
            r[a] = s[c];
            a++;
        }
    }
    co = a;
}

if (m == 1) {
    d = d.replace(/"/gi, "");
    var a = 0;
    var pat = new RegExp(d, "i");
    for (var c = 0; c < s.length; c++) {
		var rn = s[c].search(pat);
		if (rn >= 0) {
			r[a] = s[c];
			a++;
		}
	}
	co = a;

}

function return_query() {
    document.jse_Form.d.value = od;
}

function num_jse() {
    document.write(co);
    if( co == 1 )
        document.write(' result found.');
    else
        document.write(' results found.');
}

function out_jse() {
    if (co == 0) {
        document.write('Your search did not match any documents.<p>Please make sure all keywords are spelt correctly.<br>Try different or more general keywords.');
        return;
    }
    for (var a = 0; a < r.length && a < 50; a++) {
        var os = r[a].split("^");
        if (bold == 1 && m == 1) {
            var br = "<b>" + d + "</b>";
            os[2] = os[2].replace(pat, br);
        }
        if (include_num == 1) {
            document.write('<span class=smalltext>',a + 1, '.</span> <a class=mainrow href="', os[1], '">', os[0], '</a><br><span class=secondrow>', os[2], '</span><p>');
        } else {
            document.write('<a class=mainrow href="', os[1], '">', os[0], '</a><br><span class=secondrow>', os[2], '</span><p>');
        }
    }
    if( r.length >= 50 )
        document.write( '<div>Search results capped at 50</div>' );

}
