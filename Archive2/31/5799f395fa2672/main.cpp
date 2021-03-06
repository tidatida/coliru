#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using boost::lexical_cast;

struct device_id_t
{
	typedef boost::multiprecision::uint128_t base_t;
	
	device_id_t()
		: _val(0) {};
	
	explicit device_id_t(const base_t x)
		: _val(x) {};
    
	const base_t& base() const
	{
		return _val;
	}
    
    friend bool operator==(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val == rhs._val;
    }
    
    friend bool operator!=(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val != rhs._val;
    }
    
    friend bool operator<(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val < rhs._val;
    }
    
    friend bool operator>(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val > rhs._val;
    }
    
    friend bool operator<=(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val <= rhs._val;
    }
    
    friend bool operator>=(const device_id_t& lhs, const device_id_t& rhs)
    {
        return lhs._val >= rhs._val;
    }
	
    /**
     * Inserts a fixed-width, zero-padded, 128-bit, upper-case hexadecimal
     * string representing the given %device_id into %os.
	 * 
	 * Does not persistently alter any format flags on %os.
     * 
     * Example:
     *   device_id_t id(12);
     *   std::stringstream ss;
     *   ss << id;
     *   assert(ss.str() == "0000000000000000000000000000000C");
     * 
     * Can be written almost directly into a MySQL BINARY(16) field:
     *   query_bit << "UPDATE `Table` SET `Id` = UNHEX('" << id << "')";
     */
    friend std::ostream& operator<<(std::ostream& os, const device_id_t& device_id)
	{
        // Work on a different stream (but the same buffer) so that our flag changes
        // don't stick.
        std::ostream tmp_stream(os.rdbuf());
        
        // boost::multiprecision::uint128_t actually ignores the uppercase
        // flag, so we can't set nouppercase which IMO would be a far preferable
        // API: lower-case hex looks so much nicer, especially when there're 128
        // bits of it in an URL!
        tmp_stream
            << std::hex << std::right << std::noshowbase << std::uppercase
            << std::setw(32) << std::setfill('0') << device_id.base();
		
		return os;
	}
	
    /**
     * Extracts a %device_id from hexadecimal values up to 128 bits from %is
     * into %device_id.
	 * 
	 * Does not persistently alter any format flags on %is.
     * 
     * Example:
     *   std::stringstream ss("80000000000000000000000000000000");
     *   device_id_t id;
     *   ss >> id;
     *   
     *   device_id_t::base_t cmp = 1; cmp <<= 127;
     *   assert(id == cmp);
     * 
     * Can interpret a MySQL BINARY(16) field almost directly:
     *   query_bit << "SELECT HEX(`Id`) AS `Id` FROM `Table`";
     *   // ..
     *   device_id_t id = boost::lexical_cast<device_id_t>(result["Id"]);
     */
	friend std::istream& operator>>(std::istream& is, device_id_t& device_id)
	{
        // Work on a different stream (but the same buffer) so that our flag changes
        // don't stick.
        std::istream tmp_stream(is.rdbuf());
        
		tmp_stream >> std::hex >> device_id._val;
		
		return is;
	}
    
private:
    base_t _val;
};


int main()
{
    device_id_t a;
    assert(a.base() == 0);
    std::cout << "A=" << lexical_cast<std::string>(a) << std::endl;
    assert(lexical_cast<std::string>(a) == "00000000000000000000000000000000");
    assert(lexical_cast<device_id_t>("00000000000000000000000000000000") == a);
    
    device_id_t b(1);
    assert(b.base() == 1);
    std::cout << "B=" << lexical_cast<std::string>(b) << std::endl;
    assert(lexical_cast<std::string>(b) == "00000000000000000000000000000001");
    assert(lexical_cast<device_id_t>("00000000000000000000000000000001") == b);
    
    device_id_t::base_t val = 1;
    val <<= 127;
    
    device_id_t c(val);
    assert(c.base() == val);
    std::cout << "C=" <<lexical_cast<std::string>(c) << std::endl;
    assert(lexical_cast<std::string>(c) == "80000000000000000000000000000000");
    assert(lexical_cast<device_id_t>("80000000000000000000000000000000") == c);
    
    device_id_t d(12);
    assert(d.base() == 12);
    std::cout << "D=" << lexical_cast<std::string>(d) << std::endl;
    assert(lexical_cast<std::string>(d) == "0000000000000000000000000000000C");
    assert(lexical_cast<device_id_t>("0000000000000000000000000000000C") == d);
}
