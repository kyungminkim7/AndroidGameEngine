// automatically generated by the FlatBuffers compiler, do not modify

package MyGame.Example;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class StructOfStructsT {
  private MyGame.Example.AbilityT a;
  private MyGame.Example.TestT b;
  private MyGame.Example.AbilityT c;

  public MyGame.Example.AbilityT getA() { return a; }

  public void setA(MyGame.Example.AbilityT a) { this.a = a; }

  public MyGame.Example.TestT getB() { return b; }

  public void setB(MyGame.Example.TestT b) { this.b = b; }

  public MyGame.Example.AbilityT getC() { return c; }

  public void setC(MyGame.Example.AbilityT c) { this.c = c; }


  public StructOfStructsT() {
    this.a = new MyGame.Example.AbilityT();
    this.b = new MyGame.Example.TestT();
    this.c = new MyGame.Example.AbilityT();
  }
}
