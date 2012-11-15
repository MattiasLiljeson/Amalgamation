// =================================================================================================
//                                         Test
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Brief description, Lorem
///        ipsum dolor sit amet
/// 
/// # Test
/// Detailed description will then start here, describe everything
/// using lots of fancy words.
/// Edited By: Name, 2012-11-14
///-------------------------------------------------------------------------------------------------
class Test
{
public:
    ///
	/// An enum type.
    /// The documentation block cannot be put after the enum!
    ///
    enum EnumType
    {
        int EVal1, /**< enum value 1 */
        int EVal2 /**< enum value 2 */
    };
    void memberFunc(); ///< a member function.
	///
	/// Multiline for func, wip
    ///
	void anotherFunc();	
	///
	/// A normal member taking two arguments and returning an integer value.
    /// \param A an integer argument.
    /// \param s A constant character pointer.
    /// \return The test results
    ///
	int funcWithParams(int a, const char *s);
	void foo(int v /**< [in] Docs for input parameter v. */);
protected:
    int m_value; ///< A brief description of an integer value.
};