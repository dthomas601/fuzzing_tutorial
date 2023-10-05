with Interfaces.C; use Interfaces.C;

package addition_pack is

   function Add_Func (a,b : Integer) return Integer
     with
       Export        => True,
       Convention    => C,
       External_Name => "add_func";

end addition_pack;