package ffmpeg


import "testing"

func TestRational(t*testing.T){
    var R1 Rational=Rational{1,2}
    var R2 Rational=Rational{1,2}
    var R3 Rational=Rational{2,3}
    var R4 Rational=Rational()
    
    if(!R1.Equals(R2)){
	t.Error("fail !R1.Equals(R2)")
    }
    
    if(R1.Lower(R3)){
	t.Error("fail R1.Lower(R3)")
    }
    
    if(!R3.Lower(R1)){
	t.Error("fail !R3.Lower(R1)")
    }
    
    if(!R1.Greater(R3)){
	t.Error("fail !R1.Greater(R3)")
    }
    if(!R1.Greater(R4)){
	t.Error("fail !R1.Greater(R3)")
    }
}